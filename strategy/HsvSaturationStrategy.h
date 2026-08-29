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
    LineTraceRunner& mLineTraceRunner;
    DistanceCalculator& mDistanceCalculator;
    PIDCalculator& mPIDCalculator;
    ColorSensor& mColorSensor;

    uint32_t mSampleCount;
    uint8_t mMaximumSaturation;
    uint8_t mMinimumSaturation;
    uint8_t mValueAtMaximumSaturation;
    uint8_t mValueAtMinimumSaturation;
    uint8_t mMaximumValueAtOrAboveSaturationThreshold;
    bool mHasSampleAtOrAboveSaturationThreshold;

    void resetStatistics();
    void recordSample(const ColorSensor::HSV& hsv);
    void printStatistics() const;
};
