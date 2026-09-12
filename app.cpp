#include "app.h"

// 攻略パッケージ
#include "LapStrategy.h"
#include "BottleDeliveryStrategy.h"
#include "RallyStrategy.h"

// シーンパッケージ
#include "SceneManager.h"
#include "CourseParameter.h"

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

// Hub内蔵ボタンと5×5ディスプレイ（SPIKE-RT C API）
extern "C"
{
#include <spike/hub/button.h>
#include <spike/hub/display.h>
}

// ログ用
#include "Logger.h"

// タスク系
#include "kernel.h"
#include "kernel_cfg.h"

// バッテリー
#include "Battery.h"

using namespace spikeapi;

// CourseParameter.hのextern宣言に対応する、全ファイル共通のコース係数。
int COURSE_DIRECTION = DEFAULT_COURSE_DIRECTION;

namespace
{
constexpr int START_POLL_INTERVAL_US = 20 * 1000;
constexpr int REQUIRED_STABLE_SAMPLES = 2;

void showCourseSelection()
{
    const char courseLetter = COURSE_DIRECTION == 1 ? 'L' : 'R';
    const pbio_error_t result = hub_display_char(courseLetter);

    Logger::printf(
        "[app]Course=%c (%d)\n",
        courseLetter,
        COURSE_DIRECTION);

    if (result != PBIO_SUCCESS)
    {
        Logger::printf(
            "[app]Course display failed: %d\n",
            static_cast<int>(result));
    }
}

void waitForStartAndSelectCourse(ForceSensor& forceSensor)
{
    int candidateDirection = COURSE_DIRECTION;
    int stableButtonSamples = 0;
    int stableStartSamples = 0;
    bool startPressed = false;

    showCourseSelection();
    Logger::printf(
        "[app]Left button: L / Right button: R / Force sensor: Start\n");

    while (true)
    {
        // フォースセンサーの押下が確定するまではコースを選択できる。
        if (!startPressed)
        {
            hub_button_t pressed = static_cast<hub_button_t>(0);
            if (hub_button_is_pressed(&pressed) == PBIO_SUCCESS)
            {
                const bool leftPressed = (pressed & HUB_BUTTON_LEFT) != 0;
                const bool rightPressed = (pressed & HUB_BUTTON_RIGHT) != 0;

                // 両方押された場合・どちらも押されていない場合は選択を維持。
                if (leftPressed != rightPressed)
                {
                    const int selectedDirection = leftPressed ? 1 : -1;
                    if (selectedDirection != candidateDirection)
                    {
                        candidateDirection = selectedDirection;
                        stableButtonSamples = 1;
                    }
                    else if (stableButtonSamples < REQUIRED_STABLE_SAMPLES)
                    {
                        ++stableButtonSamples;
                    }

                    // 短い接点の揺れを除き、長押しではログ・表示を繰り返さない。
                    if (stableButtonSamples >= REQUIRED_STABLE_SAMPLES &&
                        COURSE_DIRECTION != candidateDirection)
                    {
                        COURSE_DIRECTION = candidateDirection;
                        showCourseSelection();
                    }
                }
                else
                {
                    stableButtonSamples = 0;
                }
            }
            else
            {
                // ボタンを取得できなかった回の値は使用しない。
                stableButtonSamples = 0;
            }
        }

        const bool touched = forceSensor.isTouched();
        if (!startPressed)
        {
            stableStartSamples = touched ? stableStartSamples + 1 : 0;
            if (stableStartSamples >= REQUIRED_STABLE_SAMPLES)
            {
                startPressed = true;
                stableStartSamples = 0;
            }
        }
        else
        {
            stableStartSamples = !touched ? stableStartSamples + 1 : 0;
            if (stableStartSamples >= REQUIRED_STABLE_SAMPLES)
            {
                break;
            }
        }

        tslp_tsk(START_POLL_INTERVAL_US);
    }

    Logger::printf(
        "[app]Course confirmed=%s (%d)\n",
        COURSE_DIRECTION == 1 ? "Left" : "Right",
        COURSE_DIRECTION);
}
} // namespace

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
        pidCalculator,
        trapezoidCalculator);

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

    /* コースを選択し、フォースセンサーを押して離すまで待つ */
    waitForStartAndSelectCourse(forceSensor);

    Logger::printf("[app]スタート\n");

    /* ラップ攻略 */
    Logger::printf("[app]ラップ開始\n");

    lapStrategy.execute();

    /* ボトルデリバリー攻略 */
    Logger::printf("[app]ボトルデリバリー開始\n");

    bottleDeliveryStrategy.execute();

    /* ETラリー攻略 */
    Logger::printf("[app]ETラリー開始\n");
    
    rallyStrategy.execute();

    Logger::printf("[app]終了\n");

    ext_tsk();
}
