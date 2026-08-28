#include "app.h"

// 攻略パッケージ
#include "LapStrategy.h"
#include "BottleDeliveryStrategy.h"
#include "RallyStrategy.h"

// シーンパッケージ
#include "SceneManager.h"

// 制御パッケージ
#include "LineTraceRunner.h"
#include "GyroTraceRunner.h"
#include "ArmController.h"

// 演算パッケージ
#include "PIDCalculator.h"
#include "TrapezoidCalculator.h"
#include "DistanceCalculator.h"

// デバイスパッケージ
#include "Motor.h"
#include "ForceSensor.h"
#include "ColorSensor.h"
#include "IMU.h"

// ログ用
#include "Logger.h"

// タスク系
#include "kernel.h"
#include "kernel_cfg.h"

// バッテリー
#include "Battery.h"

using namespace spikeapi;

/* メインタスク */
void main_task(intptr_t exinf)
{
    /* デバイス */
    Motor leftWheel(
        EPort::PORT_B,
        Motor::EDirection::COUNTERCLOCKWISE,
        true);

    Motor rightWheel(
        EPort::PORT_A,
        Motor::EDirection::CLOCKWISE,
        true);

    Motor ArmMotor(
        EPort::PORT_C,
        Motor::EDirection::COUNTERCLOCKWISE,
        true);

    ForceSensor forceSensor(EPort::PORT_D);
    ColorSensor colorSensor(EPort::PORT_E);

    Battery battery;

    IMU imu;

    imu.setTilt(51.0f);

    // IMUの初期キャリブレーション待ち
    while (!imu.isReady())
    {
        tslp_tsk(100 * 1000);
    }

    /* 演算 */
    PIDCalculator pidCalculator;

    DistanceCalculator distanceCalculator(
        leftWheel,
        rightWheel);

    TrapezoidCalculator trapezoidCalculator(
        distanceCalculator);

    /* 検出 */
    ColorDetector colorDetector(
        colorSensor);

    TargetDistanceDetector targetDistanceDetector(
        distanceCalculator);

    TargetAngleDetector targetAngleDetector(
        imu);

    TargetColorDetector targetColorDetector(
        colorDetector);

    /* 制御 */
    LineTraceRunner lineTraceRunner(
        leftWheel,
        rightWheel,
        colorSensor,
        pidCalculator);

    GyroTraceRunner gyroTraceRunner(
        leftWheel,
        rightWheel,
        distanceCalculator,
        pidCalculator,
        trapezoidCalculator,
        imu);

    ArmController armController(
        ArmMotor);

    /* シーン */
    SceneManager sceneManager(
        lineTraceRunner,
        gyroTraceRunner,
        pidCalculator,
        trapezoidCalculator,
        distanceCalculator,
        targetDistanceDetector,
        targetAngleDetector,
        targetColorDetector,
        imu);

    /* ログ */
    Logger logger(
        colorSensor,
        leftWheel,
        rightWheel);

    /* 攻略クラス */
    LapStrategy lapStrategy(
        sceneManager);

    BottleDeliveryStrategy bottleDeliveryStrategy(
        sceneManager,
        armController);

    RallyStrategy rallyStrategy(sceneManager);

    /* 初期化 */
    logger.init();

    Logger::printf("[app]接続完了\n");

    Logger::printf(
        "[app]出力電圧:%d\n",
        battery.getVoltage());

    Logger::printf(
        "[app]出力電流:%d\n",
        battery.getCurrent());

    /* アーム初期位置 */
    armController.Armreset();

    // 1回目の押下
    /* キャリブレーション用
    while (!forceSensor.isTouched());
    tslp_tsk(20 * 1000);
    while (forceSensor.isTouched());

    lineTraceRunner.calibrateTargetReflection(0);
    Logger::printf("キャリブレーション１完了\n");

    // 2回目の押下
    while (!forceSensor.isTouched());
    tslp_tsk(20 * 1000);
    while (forceSensor.isTouched());

    lineTraceRunner.calibrateTargetReflection(1);
    Logger::printf("キャリブレーション２完了\n");
    */

    /* スタート待ち */
    while (!forceSensor.isTouched());
    tslp_tsk(20 * 1000);
    while (forceSensor.isTouched());

    Logger::printf("ビルド更新2");

    Logger::printf("[app]スタート\n");

    ColorSensor::HSV hsv;
    
    /*
    while(true){
        colorSensor.getHSV(hsv);
        Logger::printf("H=%d S=%d V=%d\n",hsv.h,hsv.s,hsv.v);
        tslp_tsk(100*1000);
    }*/
 
    /*
    distanceCalculator.reset();
    imu.resetHeading();
    for(int i = 0; i<1; i++)
    {
       
        while(1){
            float current_heading = imu.getHeading();
 
            if (current_heading <= -90)
            {
                Logger::printf("%f\n",current_heading);
                leftWheel.stop();
                rightWheel.stop();
                //tslp_tsk(1000*1000);
                //Logger::printf("%f\n",current_heading);
                //distanceCalculator.reset();
                //imu.resetHeading();
                break;
            }
 
            leftWheel.setPower(-40);
            rightWheel.setPower(40);
 
            tslp_tsk(10*1000);
        }
        while(1){
            float current_heading = imu.getHeading();
            Logger::printf("%f\n",current_heading);
            tslp_tsk(1000*1000);
        }
    }*/

    /* ラップ攻略 */
    //Logger::printf("[app]ラップ開始\n");

    //lapStrategy.execute();

    /* ボトルデリバリー攻略 */
    Logger::printf("[app]ボトルデリバリー開始\n");

    bottleDeliveryStrategy.execute();

    /* ETラリー攻略 */
    Logger::printf("[app]ETラリー開始\n");

    rallyStrategy.execute();

    Logger::printf("[app]終了\n");

    ext_tsk();
}