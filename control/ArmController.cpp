#include "ArmController.h"

//コンストラクタ
ArmController::ArmController(
    Motor& armMotor)
    : mArmMotor(armMotor)
{
}

void ArmController::moveArmup()
{
     mArmMotor.setPower(50);
    tslp_tsk(350*1000);

    mArmMotor.stop();
}

void ArmController::Armreset()
{
     mArmMotor.setPower(-50);
    tslp_tsk(600*1000);

    mArmMotor.stop();
}