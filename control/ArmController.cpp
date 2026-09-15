#include "ArmController.h"
#include "kernel.h"

//コンストラクタ
ArmController::ArmController(
    Motor& armMotor)
    : mArmMotor(armMotor)
{
}

//時間があれば角度指定方式にする
void ArmController::moveArmup()
{
    mArmMotor.setPower(50);
    tslp_tsk(350*1000);

    mArmMotor.stop();
}

//時間があれば角度指定方式にする
void ArmController::Armreset()
{
    mArmMotor.setPower(-50);
    tslp_tsk(600*1000);

    mArmMotor.stop();
}