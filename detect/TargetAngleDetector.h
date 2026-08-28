#pragma once

#include "IMU.h"
#include "IEventDetector.h"

using namespace spikeapi;

class TargetAngleDetector : public IEventDetector
{
public:
    TargetAngleDetector(
        IMU& imu);

    void setTargetAngle(float angle);
    void setAngleTolerance(float tolerance);

    bool judge() override;

private:
    IMU mImu;
    float mTargetAngle;
    float mAngleTolerance;
};