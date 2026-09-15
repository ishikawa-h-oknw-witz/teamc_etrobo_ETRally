#pragma once

#include "Motor.h"

using namespace spikeapi;

class ArmController
{
public:
    ArmController(
        Motor& ArmMotor);

    //アームを前向きに制御
    void moveArmup();
    
    //初期角度にアームをリセット
    void Armreset();

private:
    Motor& mArmMotor;
};