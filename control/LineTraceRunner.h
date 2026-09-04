#ifndef LINE_TRACE_RUNNNER_H
#define LINE_TRACE_RUNNNER_H

#include "ColorSensor.h"
#include "PIDCalculator.h"
#include "TrapezoidCalculator.h"
#include "Motor.h"
#include "kernel.h"

using namespace spikeapi;

enum RunnerEdge
{
    LeftEdge = 1,
    RightEdge = -1
};

class LineTraceRunner
{
public:
    LineTraceRunner(
        Motor& leftMotor,
        Motor& rightMotor,
        ColorSensor& colorSensor,
        PIDCalculator& pidController,
        TrapezoidCalculator& trapezoidCalculate);

    //基準速度設定用のセッター
    void setBaseSpeed(int speed);

    //走行エッジ設定用のセッター
    void setEdge(RunnerEdge edge);

    //目標輝度設定用のセッター
    void setTargetSensorValue(int targetSensorValue);

    //Reflectionを使った走行
    void run();

    //停止
    void stop();

private:
    Motor& mLeftMotor;

    Motor& mRightMotor;

    ColorSensor& mColorSensor;

    PIDCalculator& mPIDCalculator;

    TrapezoidCalculator& mTrapezoidCalculator;

    int mTargetSensorValue;

    int mBaseSpeed;

    RunnerEdge mEdge = RunnerEdge::RightEdge;
};

#endif