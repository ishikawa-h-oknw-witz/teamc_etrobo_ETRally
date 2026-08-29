#include "HsvSaturationStrategy.h"

#include "Logger.h"

namespace
{
constexpr int TRACE_SPEED = 30;
constexpr int TARGET_DISTANCE_MM = 1000;
constexpr int TARGET_VALUE = 55;

constexpr float TRACE_KP = 0.4f;
constexpr float TRACE_KI = 0.0f;
constexpr float TRACE_KD = 0.2f;
}

HsvSaturationStrategy::HsvSaturationStrategy(
    LineTraceRunner& lineTraceRunner,
    DistanceCalculator& distanceCalculator,
    PIDCalculator& pidCalculator,
    ColorSensor& colorSensor)
    : mLineTraceRunner(lineTraceRunner),
      mDistanceCalculator(distanceCalculator),
      mPIDCalculator(pidCalculator),
      mColorSensor(colorSensor),
      mSampleCount(0),
      mMaximumSaturation(0),
      mMinimumSaturation(255),
      mValueAtMaximumSaturation(0),
      mValueAtMinimumSaturation(0)
{
    resetStatistics();
}

void HsvSaturationStrategy::execute()
{
    resetStatistics();

    mDistanceCalculator.reset();
    mPIDCalculator.reset();
    mPIDCalculator.setGain(
        TRACE_KP,
        TRACE_KI,
        TRACE_KD);

    mLineTraceRunner.setBaseSpeed(TRACE_SPEED);
    mLineTraceRunner.setEdge(RunnerEdge::RightEdge);
    mLineTraceRunner.setTargetSensorValue(TARGET_VALUE);

    Logger::printf(
        "[HSV計測]開始 ReflectionTrace Speed=%d Distance=%dmm\r\n",
        TRACE_SPEED,
        TARGET_DISTANCE_MM);

    while (mDistanceCalculator.getDistance() < TARGET_DISTANCE_MM)
    {
        ColorSensor::HSV hsv;

        // 反射光でライントレースし、各制御周期でHSVも取得する。
        mLineTraceRunner.run();
        mColorSensor.getHSV(hsv);
        recordSample(hsv);
    }

    finish();
    printStatistics();
}

void HsvSaturationStrategy::update(
    int sceneId,
    bool result)
{
    (void)sceneId;
    (void)result;
}

void HsvSaturationStrategy::updateNextScene()
{
}

void HsvSaturationStrategy::finish()
{
    mLineTraceRunner.stop();
}

void HsvSaturationStrategy::resetStatistics()
{
    mSampleCount = 0;
    mMaximumSaturation = 0;
    mMinimumSaturation = 255;
    mValueAtMaximumSaturation = 0;
    mValueAtMinimumSaturation = 0;
}

void HsvSaturationStrategy::recordSample(
    const ColorSensor::HSV& hsv)
{
    // 同じS値が複数回出た場合は、最初に取得したV値を残す。
    if (mSampleCount == 0 ||
        hsv.s > mMaximumSaturation)
    {
        mMaximumSaturation = hsv.s;
        mValueAtMaximumSaturation = hsv.v;
    }

    if (mSampleCount == 0 ||
        hsv.s < mMinimumSaturation)
    {
        mMinimumSaturation = hsv.s;
        mValueAtMinimumSaturation = hsv.v;
    }

    mSampleCount++;
}

void HsvSaturationStrategy::printStatistics() const
{
    if (mSampleCount == 0)
    {
        Logger::printf(
            "[HSV計測]S値を取得できませんでした\r\n");
        return;
    }

    Logger::printf(
        "[HSV計測]終了 Samples=%u\r\n",
        static_cast<unsigned int>(mSampleCount));

    Logger::printf(
        "[HSV計測]S Max=%u V=%u Min=%u V=%u\r\n",
        static_cast<unsigned int>(mMaximumSaturation),
        static_cast<unsigned int>(mValueAtMaximumSaturation),
        static_cast<unsigned int>(mMinimumSaturation),
        static_cast<unsigned int>(mValueAtMinimumSaturation));
}
