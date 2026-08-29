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
      mSaturationSum(0),
      mMaximumSaturation(0),
      mMinimumSaturation(255)
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
        recordSaturation(hsv.s);
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
    for (int index = 0;
         index < SATURATION_VALUE_COUNT;
         index++)
    {
        mSaturationHistogram[index] = 0;
    }

    mSampleCount = 0;
    mSaturationSum = 0;
    mMaximumSaturation = 0;
    mMinimumSaturation = 255;
}

void HsvSaturationStrategy::recordSaturation(
    uint8_t saturation)
{
    mSaturationHistogram[saturation]++;
    mSampleCount++;
    mSaturationSum += saturation;

    if (saturation > mMaximumSaturation)
    {
        mMaximumSaturation = saturation;
    }

    if (saturation < mMinimumSaturation)
    {
        mMinimumSaturation = saturation;
    }
}

uint32_t HsvSaturationStrategy::calculateMedianTimes10() const
{
    if (mSampleCount == 0)
    {
        return 0;
    }

    const uint32_t lowerIndex =
        (mSampleCount - 1) / 2;
    const uint32_t upperIndex =
        mSampleCount / 2;

    uint32_t cumulativeCount = 0;
    uint32_t lowerValue = 0;
    uint32_t upperValue = 0;
    bool lowerValueFound = false;

    for (uint32_t saturation = 0;
         saturation < SATURATION_VALUE_COUNT;
         saturation++)
    {
        cumulativeCount +=
            mSaturationHistogram[saturation];

        if (!lowerValueFound &&
            cumulativeCount > lowerIndex)
        {
            lowerValue = saturation;
            lowerValueFound = true;
        }

        if (cumulativeCount > upperIndex)
        {
            upperValue = saturation;
            break;
        }
    }

    // (lower + upper) / 2 を小数第1位まで表す。
    return (lowerValue + upperValue) * 5;
}

void HsvSaturationStrategy::printStatistics() const
{
    if (mSampleCount == 0)
    {
        Logger::printf(
            "[HSV計測]S値を取得できませんでした\r\n");
        return;
    }

    const uint32_t averageTimes10 =
        static_cast<uint32_t>(
            (mSaturationSum * 10 + mSampleCount / 2) /
            mSampleCount);

    const uint32_t medianTimes10 =
        calculateMedianTimes10();

    Logger::printf(
        "[HSV計測]終了 Samples=%u\r\n",
        static_cast<unsigned int>(mSampleCount));

    Logger::printf(
        "[HSV計測]S Max=%u Average=%u.%u Median=%u.%u Min=%u\r\n",
        static_cast<unsigned int>(mMaximumSaturation),
        static_cast<unsigned int>(averageTimes10 / 10),
        static_cast<unsigned int>(averageTimes10 % 10),
        static_cast<unsigned int>(medianTimes10 / 10),
        static_cast<unsigned int>(medianTimes10 % 10),
        static_cast<unsigned int>(mMinimumSaturation));
}
