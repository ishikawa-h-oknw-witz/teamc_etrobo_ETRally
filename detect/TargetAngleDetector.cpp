#include "TargetAngleDetector.h"
#include <stdlib.h>

TargetAngleDetector::TargetAngleDetector(
    IMU& imu)
    : mImu(imu),
      mTargetAngle(0)
{
}

void TargetAngleDetector::setTargetAngle(float angle)
{
    mTargetAngle = angle;
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