#include "BottleDeliveryStrategy.h"
#include "Logger.h"
#include "kernel.h"

const SceneOrder BottleDeliveryStrategy::EnterBottle[] =
{
    {0, static_cast<int>(MoveSceneID::MoveToBottlePosition), ActionType::Move}, // ボトル前まで移動
    {1, static_cast<int>(StopSceneID::CheckCount),           ActionType::Stop}  // 回数確認用
};


const SceneOrder BottleDeliveryStrategy::DetectBottleColor[] =
{
    {0, static_cast<int>(BottleDetectSceneID::DetectYellowBottle), ActionType::BottleDetect}, // 黄ボトル検知
    {1, static_cast<int>(BottleDetectSceneID::DetectBlueBottle),   ActionType::BottleDetect}, // 青ボトル検知
    {2, static_cast<int>(BottleDetectSceneID::DetectRedBottle),    ActionType::BottleDetect}  // 赤ボトル検知
};


const SceneOrder BottleDeliveryStrategy::EnterZone[] =
{
    {0, static_cast<int>(TurnSceneID::AdjustEnterAngle),    ActionType::Turn},      // 角度調整
    {1, static_cast<int>(LineTraceSceneID::EnterCurve1),    ActionType::LineTrace}, // Dlvカーブ1
    {2, static_cast<int>(LineTraceSceneID::EnterCurve2),    ActionType::LineTrace}, // Dlvカーブ2
    {3, static_cast<int>(LineTraceSceneID::PassBlueLine),   ActionType::LineTrace}, // Dlv行き青スルー
    {4, static_cast<int>(LineTraceSceneID::EnterStraight1), ActionType::LineTrace}, // Dlv直線1
    {5, static_cast<int>(LineTraceSceneID::EnterCurve3),    ActionType::LineTrace}  // Dlvカーブ3
};


const SceneOrder BottleDeliveryStrategy::MoveZone[] =
{
    {0, static_cast<int>(LineTraceSceneID::ApproachYellowArea), ActionType::LineTrace}, // 黄エリア前まで
    {1, static_cast<int>(LineTraceSceneID::ApproachBlueArea),   ActionType::LineTrace}, // 青エリア前まで
    {2, static_cast<int>(LineTraceSceneID::ApproachRedArea),    ActionType::LineTrace}  // 赤エリア前まで
};


const SceneOrder BottleDeliveryStrategy::CarryZone[] =
{
    {0, static_cast<int>(TurnSceneID::Turn30Right),            ActionType::Turn}, // 右に30°回転
    {1, static_cast<int>(MoveSceneID::MoveToDeliveryArea),     ActionType::Move}, // Dlvエリアまで

    {2, static_cast<int>(TurnSceneID::Turn30Right),            ActionType::Turn}, // 右に30°回転
    {3, static_cast<int>(MoveSceneID::MoveToDeliveryArea),     ActionType::Move}, // Dlvエリアまで

    {4, static_cast<int>(TurnSceneID::Turn30Right),            ActionType::Turn}, // 右に30°回転
    {5, static_cast<int>(MoveSceneID::MoveToDeliveryArea),     ActionType::Move}, // Dlvエリアまで
    
    {6, static_cast<int>(MoveSceneID::ReturnToDeliveryLine),   ActionType::Move}, // Dlv線まで帰還
    {7, static_cast<int>(TurnSceneID::Turn90Right),            ActionType::Turn}  // 右に90°回転
};


const SceneOrder BottleDeliveryStrategy::ReturnZone[] =
{
    {0, static_cast<int>(LineTraceSceneID::ReturnFromYellowArea), ActionType::LineTrace}, // Dlv黄から行きゲート前まで
    {1, static_cast<int>(LineTraceSceneID::ReturnFromBlueArea),   ActionType::LineTrace}, // Dlv青から行きゲート前まで
    {2, static_cast<int>(LineTraceSceneID::ReturnFromRedArea),    ActionType::LineTrace}  // Dlv赤から行きゲート前まで
};


const SceneOrder BottleDeliveryStrategy::EnterRally[] =
{
    {0, static_cast<int>(LineTraceSceneID::ReturnCurve1),      ActionType::LineTrace}, // Dlv帰還カーブ1
    {1, static_cast<int>(LineTraceSceneID::ReturnToBlue),      ActionType::LineTrace}, // Dlv帰還青まで
    {2, static_cast<int>(LineTraceSceneID::ReturnBlueHalfway), ActionType::LineTrace}, // Dlv青半分まで
    {3, static_cast<int>(TurnSceneID::Turn90Right),            ActionType::Turn},      // Dlv右に90°回転
    {4, static_cast<int>(MoveSceneID::ReturnToBaseline),       ActionType::Move},      // Dlv基準線まで
    {5, static_cast<int>(StopSceneID::Finish),                 ActionType::Stop}
};

BottleDeliveryStrategy::BottleDeliveryStrategy(
    SceneManager& sceneManager,
    ArmController& armController)
    : mSceneManager(sceneManager),
      mArmController(armController),
      mSkipCount(-1)
{
}


void BottleDeliveryStrategy::execute()
{
    // アーム上昇
    tslp_tsk(100 * 1000);

    mArmController.moveArmup();

    // アーム上昇直後の振動が収まるまで待つ
    tslp_tsk(200 * 1000);

    // ボトル前まで移動
    changeScene(EnterBottle, 1);

    // ボトル色検知
    const char* colorName[] = {"黄", "青", "赤"};

    mSkipCount = -1;

    for (int sceneNum = 0; sceneNum < 3; sceneNum++)
    {
        const SceneOrder& sceneOrder =
            DetectBottleColor[sceneNum];

        mSceneManager.setActionType(
            sceneOrder.actionType);

        mSceneManager.setSceneID(
            sceneOrder.sceneId);

        Logger::printf(
            "[BottleDelivery]SceneID=%d\n",
            sceneOrder.sceneId);

        if (mSceneManager.SceneExecute())
        {
            mSkipCount = sceneNum;

            Logger::printf(
                "[BottleDelivery]色検知:%s\n",
                colorName[sceneNum]);

            break;
        }
    }

    // ボトル検知失敗
    if (mSkipCount < 0)
    {
        tslp_tsk(200 * 1000);

        Logger::printf(
            "[BottleDelivery]ボトル検知失敗\n");

        changeScene(EnterBottle, 1);
    }

    // アーム下降
    mArmController.Armreset();

    // Dlvカーブ3まで
    changeScene(EnterZone, 5);

    // 検知した色のエリアまで移動
    changeScene(&MoveZone[mSkipCount], 0);

    // ボトル設置
    changeScene(CarryZone, 7);

    // 検知した色に対応する帰還
    changeScene(&ReturnZone[mSkipCount], 0);

    //ラリーへ向かう
    changeScene(EnterRally, 5);
}


void BottleDeliveryStrategy::update(
    int sceneId,
    bool result)
{
    // 攻略状態を更新
}


void BottleDeliveryStrategy::updateNextScene()
{
    // 次のシーンIDを更新
}


void BottleDeliveryStrategy::finish()
{
    // ボトルデリバリー攻略終了処理
}


void BottleDeliveryStrategy::changeScene(
    const SceneOrder sceneOrder[],
    int maxSceneNum)
{
    int sceneNum = 0;

    while (true)
    {
        const SceneOrder& sceneOrderData =
            sceneOrder[sceneNum];

        mSceneManager.setActionType(
            sceneOrderData.actionType);

        mSceneManager.setSceneID(
            sceneOrderData.sceneId);

        if (mSceneManager.SceneExecute())
        {
            sceneNum++;
        }

        if (sceneNum > maxSceneNum)
        {
            break;
        }
    }
}