#pragma once

#include <cstdint>

#include "IChallengeStrategy.h"
#include "LineTraceRunner.h"
#include "DistanceCalculator.h"
#include "PIDCalculator.h"
#include "ColorSensor.h"

/**
 * 低速で一定距離をライントレースし、走行中のHSVのS値を集計する。
 */
class HsvSaturationStrategy : public IChallengeStrategy
{
public:
    HsvSaturationStrategy(
        LineTraceRunner& lineTraceRunner,
        DistanceCalculator& distanceCalculator,
        PIDCalculator& pidCalculator,
        ColorSensor& colorSensor);

    void execute() override;
    void update(int sceneId, bool result) override;
    void updateNextScene() override;
    void finish() override;

private:
    static constexpr int SATURATION_VALUE_COUNT = 256;

    LineTraceRunner& mLineTraceRunner;
    DistanceCalculator& mDistanceCalculator;
    PIDCalculator& mPIDCalculator;
    ColorSensor& mColorSensor;

    // 1,000mm走行で必要な容量に抑え、組込み環境のスタック消費を減らす。
    uint16_t mSaturationHistogram[SATURATION_VALUE_COUNT];
    uint32_t mSampleCount;
    uint64_t mSaturationSum;
    uint8_t mMaximumSaturation;
    uint8_t mMinimumSaturation;

    void resetStatistics();
    void recordSaturation(uint8_t saturation);
    uint32_t calculateMedianTimes10() const;
    void printStatistics() const;
};
