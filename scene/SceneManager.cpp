#include "SceneManager.h"
#include "RobotParameter.h"
#include "CourseParameter.h"
#include "Logger.h"
#include "kernel.h"

namespace
{
    constexpr int COLOR_SAMPLE_COUNT = 10;
    constexpr int COLOR_REQUIRED_MATCH_COUNT = 6;
    constexpr int COLOR_SAMPLE_INTERVAL_MS = 1;
    constexpr int CONTROL_INTERVAL_MS = 4;
    constexpr int MAX_SCENE_CONTROL_CYCLES = 3000;
}

//コンストラクタ
SceneManager::SceneManager(
    LineTraceRunner& lineTraceRunner,
    GyroTraceRunner& gyroTraceRunner,
    PIDCalculator& pidCalculator,
    TrapezoidCalculator& trapezoidCalculator,
    DistanceCalculator& distanceCalculator,
    TargetDistanceDetector& targetDistanceDetector,
    TargetAngleDetector& targetAngleDetector,
    TargetColorDetector& targetColorDetector,
    IMU& imu
    )
    : mLineTraceRunner(lineTraceRunner),
      mGyroTraceRunner(gyroTraceRunner),
      mPIDCalculator(pidCalculator),
      mTrapezoidCalculator(trapezoidCalculator),
      mDistanceCalculator(distanceCalculator),
      mTargetDistanceDetector(targetDistanceDetector),
      mTargetAngleDetector(targetAngleDetector),
      mTargetColorDetector(targetColorDetector),
      mImu(imu),
      mSceneId(0),
      mEventDetector(nullptr)
{
}

int SceneManager::getSceneID()
{
    return mSceneId;
}

void SceneManager::setSceneID(int sceneId)
{
    mSceneId = sceneId;
}

void SceneManager::setActionType(ActionType actiontype)
{
    mActionType = actiontype;
}

bool SceneManager::SceneExecute()
{
    mImu.resetHeading();
    mEventDetector = nullptr;
    setParameter();

    mDistanceCalculator.reset();
    mPIDCalculator.reset();

    if (mActionType == ActionType::BottleDetect)
    {
        return mTargetColorDetector.judgeMultiple(
            COLOR_SAMPLE_COUNT,
            COLOR_REQUIRED_MATCH_COUNT,
            COLOR_SAMPLE_INTERVAL_MS);
    }

    if (mActionType == ActionType::ColorDetect)
    {
        return mTargetColorDetector.judgeMultiple(
            COLOR_SAMPLE_COUNT,
            COLOR_REQUIRED_MATCH_COUNT,
            COLOR_SAMPLE_INTERVAL_MS);
    }

    if (mActionType == ActionType::Stop)
    {
        mGyroTraceRunner.stop();
        return true;
    }

    if (mEventDetector == nullptr)
    {
        Logger::printf("Event detector is not configured. SceneID=%d\r\n", mSceneId);
        mGyroTraceRunner.stop();
        return false;
    }
    
    int controlCycleCount = 0;
    while(!mEventDetector->judge())
    {
        if (controlCycleCount >= MAX_SCENE_CONTROL_CYCLES)
        {
            Logger::printf("Scene timeout. SceneID=%d\r\n", mSceneId);
            mGyroTraceRunner.stop();
            return false;
        }

        // 走行実行
        switch (mActionType)
        {
        case ActionType::LineTrace:
            mLineTraceRunner.run();
            break;

        case ActionType::VLineTrace:
            mLineTraceRunner.vrun();
            break;

        case ActionType::Move:
            mGyroTraceRunner.move();
            break;

        case ActionType::Turn:
            mGyroTraceRunner.turn();
            break;

        default:
            break;
        }
        tslp_tsk(CONTROL_INTERVAL_MS * 1000);
        controlCycleCount++;
    }

    // シーン終了
    return true;
}

void SceneManager::setParameter()
{

    switch(mActionType)
    {
    case ActionType::VLineTrace:
    case ActionType::LineTrace:
    {
        const LineTraceScene& linetracescene = lineTraceScenes[mSceneId];

        //台形計算
        mTrapezoidCalculator.setParameter(linetracescene.trapezoidParameter);

        // PID
        mPIDCalculator.setGain(
            linetracescene.pid.kp,
            linetracescene.pid.ki,
            linetracescene.pid.kd);

        // Leftコース基準のエッジを、選択したコースへ変換する。
        // RunnerEdgeはLeftEdge=1、RightEdge=-1。
        const RunnerEdge courseEdge = static_cast<RunnerEdge>(
            static_cast<int>(linetracescene.edge) * COURSE_DIRECTION);
        mLineTraceRunner.setEdge(courseEdge);

        // 目標輝度
        /*
        if (linetracescene.targetSensorValue == CalibrationData::BlackWhiteCenter)
        {
            mLineTraceRunner.setTargetSensorValue(
                mLineTraceRunner.getTargetSensorValue(0));
        }
        else
        {
            mLineTraceRunner.setTargetSensorValue(
                mLineTraceRunner.getTargetSensorValue(1));
        }
        */

        // 走行距離
        if (linetracescene.targetDistance != 0)
        {
            mTargetDistanceDetector.setTargetDistance(linetracescene.targetDistance);
            mEventDetector = &mTargetDistanceDetector;
        }

        //判定色
        if (linetracescene.finishColor[0] != Color::None)
        {
            mTargetColorDetector.setTargetColors(linetracescene.finishColor);
            mEventDetector = &mTargetColorDetector;
        }

        break;
    }
    case ActionType::Move:
    {
        const MoveScene& movescene = moveScenes[mSceneId];

        //向き
        mGyroTraceRunner.setDirection(movescene.direction);

        //台形計算
        mTrapezoidCalculator.setParameter(movescene.trapezoidParameter);

        //PID
        mPIDCalculator.setGain(
            movescene.pid.kp,
            movescene.pid.ki,
            movescene.pid.kd);

        //走行距離
        if (movescene.targetDistance != 0)
        {
            mTargetDistanceDetector.setTargetDistance(movescene.targetDistance);
            mEventDetector = &mTargetDistanceDetector;
        }

        //判定色
        if (movescene.finishColor[0] != Color::None)
        {
            mTargetColorDetector.setTargetColors(movescene.finishColor);
            mEventDetector = &mTargetColorDetector;
        }

        break;
    }
    case ActionType::Turn:
    {
        const TurnScene& turnscene = turnScenes[mSceneId];
        const float courseTargetAngle =
            turnscene.targetAngle * COURSE_DIRECTION;

        //PID
        mPIDCalculator.setGain(
            turnscene.pid.kp,
            turnscene.pid.ki,
            turnscene.pid.kd);
        
        if (courseTargetAngle != 0)
        {
            // 旋回制御と終了判定で、同じ反転後の角度を使用する。
            mGyroTraceRunner.setTargetAngle(courseTargetAngle);
            mTargetAngleDetector.setTargetAngle(courseTargetAngle);
            mEventDetector = &mTargetAngleDetector;
        }

        break;
    }
    case ActionType::BottleDetect:
    {
        const BottleDetectScene& bottledetectscene = bottleDetectScenes[mSceneId];
    
        mTargetColorDetector.setTargetColors(bottledetectscene.detectColor);
        mEventDetector = &mTargetColorDetector;

        break;
    }
    case ActionType::ColorDetect:
    {
        const ColorDetectScene& colorDetectScene = colorDetectScenes[mSceneId];

        mTargetColorDetector.setTargetColors(colorDetectScene.detectColor);
        mEventDetector = &mTargetColorDetector;

        break;
    }
    default:
        break;
    }
}
