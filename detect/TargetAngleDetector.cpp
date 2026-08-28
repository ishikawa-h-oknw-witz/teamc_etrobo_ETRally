#include "TargetAngleDetector.h"
#include <stdlib.h>

TargetAngleDetector::TargetAngleDetector(
    IMU& imu)
    : mImu(imu),
      mTargetAngle(0),
      mAngleTolerance(0.5f)
{
}

void TargetAngleDetector::setTargetAngle(float angle)
{
    mTargetAngle = angle;
}

void TargetAngleDetector::setAngleTolerance(float tolerance)
{
    mAngleTolerance = tolerance;
}

bool TargetAngleDetector::judge()
{
    float currentAngle = mImu.getHeading();

    if (mTargetAngle >= 0)
    {
        return currentAngle >= mTargetAngle;
    }
    else
    {
        return currentAngle <= mTargetAngle;
    }
}