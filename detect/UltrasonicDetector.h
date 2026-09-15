#pragma once

#include "UltrasonicSensor.h"

using namespace spikeapi;

class UltrasonicDetector
{
public:
    UltrasonicDetector(UltrasonicSensor& UltSonic);

    bool judge();

private:
    UltrasonicSensor& mUltrasonic;
};