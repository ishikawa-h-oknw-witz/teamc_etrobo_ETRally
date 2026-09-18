#include "BottleDeliveryStrategy.h"
#include "Logger.h"
#include "kernel.h"

const SceneOrder BottleDeliveryStrategy::EnterBottle[] =
{
    {0, static_cast<int>(MoveSceneID::MoveToBottlePosition), ActionType::Move}, // ボトル前まで移動
    {1, static_cast<int>(StopSceneID::CheckCount),           ActionType::Stop}  // 回数確認用
};

const SceneOrder BottleDeliveryStrategy::RecoveryBottleDetect[] =
{
    {1, static_cast<int>(MoveSceneID::RecoveryBottleDetectFront), ActionType::Move}, // ボトル前まで移動
    {2, static_cast<int>(StopSceneID::CheckCount),           ActionType::Stop}  // 回数確認用
};

const SceneOrder BottleDeliveryStrategy::RecoveryBack[] =
{
    {0, static_cast<int>(MoveSceneID::RecoveryBottleDetectBack), ActionType::Move},
    {1, static_cast<int>(StopSceneID::CheckCount),           ActionType::Stop}  // 回数確認用
};

const SceneOrder BottleDeliveryStrategy::DetectBottleColor[] =
{
    {0, static_cast<int>(BottleDetectSceneID::DetectYellowBottle), ActionType::BottleDetect}, // 黄ボトル検知
    {1, static_cast<int>(BottleDetectSceneID::DetectBlueBottle),   ActionType::BottleDetect}, // 青ボトル検知
    {2, static_cast<int>(BottleDetectSceneID::DetectRedBottle),    ActionType::BottleDetect}  // 赤ボトル検知
};

const SceneOrder BottleDeliveryStrategy::Back[] =
{
    {0, static_cast<int>(MoveSceneID::back), ActionType::Move}, // ボトル前まで移動
    {1, static_cast<int>(StopSceneID::stop),ActionType::Stop}  // 停止用
};

const SceneOrder BottleDeliveryStrategy::EnterZone[] =
{
    //{0, static_cast<int>(LineTraceSceneID::PassBlueLine),   ActionType::LineTrace}, // Dlv行き青スルー
    {0, 32, ActionType::Move},
    {1, static_cast<int>(TurnSceneID::Turn45Left), ActionType::Turn},
    {2, static_cast<int>(LineTraceSceneID::PassBlueLine), ActionType::LineTrace}, // Dlv直線1
    {3, static_cast<int>(LineTraceSceneID::EnterStraight1),    ActionType::LineTrace}  // Dlvカーブ3
};


const SceneOrder BottleDeliveryStrategy::MoveZone[] =
{
    /*
    {0, static_cast<int>(LineTraceSceneID::ApproachYellowArea), ActionType::LineTrace}, // 黄エリア前まで
    {1, static_cast<int>(LineTraceSceneID::ApproachBlueArea),   ActionType::LineTrace}, // 青エリア前まで
    {2, static_cast<int>(LineTraceSceneID::ApproachRedArea),    ActionType::LineTrace}  // 赤エリア前まで
    */
    {0, static_cast<int>(TurnSceneID::Turn60Left), ActionType::Turn},
    {1, 33, ActionType::Move},
    {2, 36, ActionType::Move},
    {3, 18, ActionType::Turn},

    {4, 19, ActionType::Turn},
    {5, 34, ActionType::Move},
    {6, 37, ActionType::Move},
    {7, 20, ActionType::Turn},

    {8, 21, ActionType::Turn},
    {9, 35, ActionType::Move},
    {10, 38, ActionType::Move},
    {11, 22, ActionType::Turn},
};


const SceneOrder BottleDeliveryStrategy::CarryZone[] =
{
    /*
    {0, static_cast<int>(TurnSceneID::Turn45Right),            ActionType::Turn}, // 右に45°回転
    {1, static_cast<int>(MoveSceneID::MoveToDeliveryArea),     ActionType::Move}, // Dlvエリアまで

    {2, static_cast<int>(TurnSceneID::Turn45Right),            ActionType::Turn}, // 右に45°回転
    {3, static_cast<int>(MoveSceneID::MoveToDeliveryArea),     ActionType::Move}, // Dlvエリアまで
    
    {4, static_cast<int>(MoveSceneID::ReturnToDeliveryLine),   ActionType::Move}, // Dlv線まで帰還
    {5, static_cast<int>(TurnSceneID::Turn90Right),            ActionType::Turn}  // 右に90°回転
    */
    {0, static_cast<int>(TurnSceneID::Turn60Left), ActionType::Turn},
};


const SceneOrder BottleDeliveryStrategy::ReturnZone[] =
{
    {0, static_cast<int>(LineTraceSceneID::ReturnFromYellowArea), ActionType::LineTrace}, // Dlv黄から行きゲート前まで
    {1, static_cast<int>(LineTraceSceneID::ReturnFromBlueArea),   ActionType::LineTrace}, // Dlv青から行きゲート前まで
    {2, static_cast<int>(LineTraceSceneID::ReturnFromRedArea),    ActionType::LineTrace}  // Dlv赤から行きゲート前まで
};


const SceneOrder BottleDeliveryStrategy::EnterRally[] =
{
    /*
    {0, static_cast<int>(LineTraceSceneID::ReturnCurve1),      ActionType::LineTrace}, // Dlv帰還カーブ1
    {1, static_cast<int>(LineTraceSceneID::ReturnToBlue),      ActionType::LineTrace}, // Dlv帰還青まで
    {2, static_cast<int>(LineTraceSceneID::ReturnBlueHalfway), ActionType::LineTrace}, // Dlv青線半分まで
    {3, static_cast<int>(TurnSceneID::Turn90Right),            ActionType::Turn},      // Dlv右に90°回転
    {4, static_cast<int>(MoveSceneID::ReturnToBaseline),       ActionType::Move},      // Dlv基準線まで
    {5, static_cast<int>(StopSceneID::Finish),                 ActionType::Stop}
    */
    {0, static_cast<int>(LineTraceSceneID::ReturnToBlue),      ActionType::LineTrace}, // Dlv帰還青まで
    {1, static_cast<int>(LineTraceSceneID::ReturnBlueHalfway), ActionType::LineTrace}, // Dlv青線半分まで
    {2, static_cast<int>(TurnSceneID::Turn90Right),            ActionType::Turn},      // Dlv右に90°回転
    {3, static_cast<int>(MoveSceneID::ReturnToBaseline),       ActionType::Move},      // Dlv基準線まで
    {4, static_cast<int>(StopSceneID::Finish),                 ActionType::Stop}
};

const SceneOrder BottleDeliveryStrategy::BottleCatch[] =
{
    {0, static_cast<int>(TurnSceneID::Turn45Left), ActionType::Turn},
    {1, 32, ActionType::Move},
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
    changeScene(BottleCatch, 1);
    changeScene(Back, 1);

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

    bool recoveryflag = false;
    static int recoverycount = 0;
    while(true){
        for (int sceneNum = 0; sceneNum < 3; sceneNum++)
        {
            const SceneOrder& sceneOrder =
                DetectBottleColor[sceneNum];

            mSceneManager.setActionType(
                sceneOrder.actionType);

            mSceneManager.setSceneID(
                sceneOrder.sceneId);

            LOG_PRINTF(
                "[BottleDelivery]SceneID=%d\n",
                sceneOrder.sceneId);

            if (mSceneManager.SceneExecute())
            {
                mSkipCount = sceneNum;

                LOG_PRINTF(
                    "[BottleDelivery]色検知:%s\n",
                    colorName[sceneNum]);

                break;
            }
        }

        // ボトル検知失敗
        if (mSkipCount < 0)
        {
            recoveryflag = true;
            recoverycount++;

            tslp_tsk(200 * 1000);

            LOG_PRINTF(
                "[BottleDelivery]ボトル検知失敗\n");

            changeScene(RecoveryBottleDetect, 1);
            continue;
        }
        break;
    }

    if(recoveryflag)
    {
        for(int i = 0; i <= recoverycount; i++){
            changeScene(RecoveryBack, 1);
        }
        recoveryflag = false;
    }

    changeScene(Back, 1);

    // アーム下降
    mArmController.Armreset();

    // Dlvカーブ3まで
    changeScene(EnterZone, 3);

    // 検知した色のエリアまで移動
    switch (mSkipCount)
    {
    case 0:
        changeScene(&MoveZone[0], 3);
        break;
    
    case 1:
        changeScene(&MoveZone[4], 3);
        break;

    case 2:
        changeScene(&MoveZone[8], 3);
        break;

    default:
        break;
    }
    //changeScene(&MoveZone[mSkipCount], 0);

    // ボトル設置
    //changeScene(CarryZone, 5);

    // 検知した色に対応する帰還
    //changeScene(&ReturnZone[mSkipCount], 0);

    //ラリーへ向かう
    changeScene(EnterRally, 4);
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