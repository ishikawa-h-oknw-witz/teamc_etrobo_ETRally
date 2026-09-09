#ifndef ARM_CONTROLLER_H
#define ARM_CONTROLLER_H

#include "Motor.h"
#include "kernel.h"

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

#endif