#include "ArmController.h"

//コンストラクタ
ArmController::ArmController(
    Motor& armMotor)
    : mArmMotor(armMotor)
{
}

void ArmController::setMaxAngle(int maxAngle)
{
    if(maxAngle > 90)
    {
        mMaxAngle = 90;
    }
    else
    {
        mMaxAngle = maxAngle;
    }
}

void ArmController::resetAngle()
{
    mArmMotor.resetCount();
}

void ArmController::moveArm(int angle)
{
    // 最大角度を超えないようにする
    if(angle > mMaxAngle)
    {
        angle = mMaxAngle;
    }

    // 現在位置を0°にする
    resetAngle();

    // 指定角度まで動かす
    if(angle > 0)
    {
        mArmMotor.setSpeed(100);

        while(mArmMotor.getCount() < angle)
        {
            // 指定角度に到達するまで待機
        }
    }

    // 到達したら停止
    mArmMotor.stop();
}

void ArmController::moveArmup()
{
     mArmMotor.setPower(50);
    tslp_tsk(450*1000);

    mArmMotor.stop();
}

void ArmController::Armreset()
{
     mArmMotor.setPower(-50);
    tslp_tsk(600*1000);

    mArmMotor.stop();
}