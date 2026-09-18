#pragma once

#define ENABLE_LOG 0

#include "Motor.h"
#include "IMU.h"
#include "ColorSensor.h"

using namespace spikeapi;

class Logger
{
public:
    Logger(ColorSensor& colorSensor,
           Motor& leftMotor,
           Motor& rightMotor);

    void output();

    static void init();
    static void printf(const char* fmt, ...);

private:
    ColorSensor& mColorSensor;
    Motor& mLeftMotor;
    Motor& mRightMotor;
    IMU mImu;
};

#if ENABLE_LOG
#define LOG_INIT() Logger::init()
#define LOG_PRINTF(...) Logger::printf(__VA_ARGS__)
#else
#define LOG_INIT()
#define LOG_PRINTF(...)
#endif
