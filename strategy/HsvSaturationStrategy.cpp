#include "HsvSaturationStrategy.h"

#include "Logger.h"

namespace
{
constexpr int TRACE_SPEED = 30;
constexpr int TARGET_DISTANCE_MM = 150;
constexpr int TARGET_VALUE = 45;
constexpr uint8_t SATURATION_THRESHOLD = 25;
constexpr RunnerEdge TRACE_EDGE = RunnerEdge::RightEdge;

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
      mValueAtMinimumSaturation(0),
      mMaximumValueAtOrAboveSaturationThreshold(0),
      mHasSampleAtOrAboveSaturationThreshold(false)
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
    mLineTraceRunner.setEdge(TRACE_EDGE);
    mLineTraceRunner.setTargetSensorValue(TARGET_VALUE);

    Logger::printf(
        "[HSV計測]開始 ReflectionTrace Speed=%d Distance=%dmm Edge=%s\r\n",
        TRACE_SPEED,
        TARGET_DISTANCE_MM,
        TRACE_EDGE == RunnerEdge::RightEdge ? "Right" : "Left");

    // CSV保存後にRecord列でHSV_DATAを抽出できる形式で出力する。
    Logger::printf(
        "Record,Sample,Distance_mm,S,V\r\n");

    while (mDistanceCalculator.getDistance() < TARGET_DISTANCE_MM)
    {
        ColorSensor::HSV hsv;

        // 反射光でライントレースし、各制御周期でHSVも取得する。
        mLineTraceRunner.run();
        mColorSensor.getHSV(hsv);

        Logger::printf(
            "HSV_DATA,%u,%d,%u,%u\r\n",
            static_cast<unsigned int>(mSampleCount + 1),
            static_cast<int>(mDistanceCalculator.getDistance()),
            static_cast<unsigned int>(hsv.s),
            static_cast<unsigned int>(hsv.v));

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
    mMaximumValueAtOrAboveSaturationThreshold = 0;
    mHasSampleAtOrAboveSaturationThreshold = false;
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

    if (hsv.s >= SATURATION_THRESHOLD &&
        (!mHasSampleAtOrAboveSaturationThreshold ||
         hsv.v > mMaximumValueAtOrAboveSaturationThreshold))
    {
        mMaximumValueAtOrAboveSaturationThreshold = hsv.v;
        mHasSampleAtOrAboveSaturationThreshold = true;
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

    if (mHasSampleAtOrAboveSaturationThreshold)
    {
        Logger::printf(
            "[HSV計測]S>=%u V Max=%u\r\n",
            static_cast<unsigned int>(SATURATION_THRESHOLD),
            static_cast<unsigned int>(
                mMaximumValueAtOrAboveSaturationThreshold));
    }
    else
    {
        Logger::printf(
            "[HSV計測]S>=%u Sample=None\r\n",
            static_cast<unsigned int>(SATURATION_THRESHOLD));
    }
}
