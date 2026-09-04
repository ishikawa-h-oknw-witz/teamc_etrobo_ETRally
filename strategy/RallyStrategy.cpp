#include "RallyStrategy.h"
#include "Logger.h"

namespace
{
constexpr int RIGHT_EDGE_INDEX = 0;
constexpr int LEFT_EDGE_INDEX = 1;
constexpr int TURN_AROUND_INDEX = 2;

constexpr int UP_LEFT_TURN_INDEX = 0;
constexpr int DOWN_RIGHT_TURN_INDEX = 1;

constexpr int UP_REJOIN_TURN_INDEX = 0;
constexpr int DOWN_REJOIN_TURN_INDEX = 1;

constexpr int VERTICAL_GATE_TURN_INDEX = 2;

constexpr int MAX_POINT_SEARCH_COUNT = 20;

// ColorDetectScene の SceneID
constexpr int YELLOW_COLOR_SCENE_ID = 0;
constexpr int BLUE_COLOR_SCENE_ID   = 1;
constexpr int RED_COLOR_SCENE_ID    = 2;
constexpr int GREEN_COLOR_SCENE_ID  = 3;

constexpr int COLOR_DETECT_SCENE_COUNT = 4;


// 基準点の色の順番を取得
int getPointOrder(Color color)
{
    switch (color)
    {
    case Color::Green:
        return 0;

    case Color::Yellow:
        return 1;

    case Color::Red:
        return 2;

    case Color::Blue:
        return 3;

    default:
        return -1;
    }
}


// 現在の基準点から次の基準点へ進むときに
// 使用するエッジを決定する
int getNextEdgeIndex(
    Color currentColor,
    Color nextColor,
    int& edgeIndex)
{
    const int currentOrder = getPointOrder(currentColor);

    const int nextOrder = getPointOrder(nextColor);

    if (currentOrder < 0 || nextOrder < 0)
    {
        return -1;
    }
    else if(currentOrder == nextOrder)
    {
        return 1;
    }

    // 基準点の並びは
    // 下から Green → Yellow → Red → Blue。
    //
    // 上方向へ進む場合：右エッジ
    // 下方向へ進む場合：左エッジ

    edgeIndex =
        nextOrder > currentOrder
        ? RIGHT_EDGE_INDEX
        : LEFT_EDGE_INDEX;

    return 2;
}


struct GatePosition
{
    Color pointColor;
    int gatePositionNum;
};


// ラリーで攻略するゲート
const GatePosition gatePositions[] =
{
    {Color::Green, 11},
    {Color::Yellow, 9},
    {Color::Red, 1},
};


// ============================================================
// ラリー用シーン
// ============================================================

// 次の基準点までライントレース
const SceneOrder EnterPoint[] =
{
    {0, static_cast<int>(LineTraceSceneID::RightEdgeLineTrace), ActionType::LineTrace},
    {1, static_cast<int>(LineTraceSceneID::LeftEdgeLineTrace),  ActionType::LineTrace},
};


// 目標基準点の中央まで移動
const SceneOrder MovePointCenter[] =
{
    {0, static_cast<int>(MoveSceneID::MoveToPointCenter), ActionType::Move},
};

// 目標ではない基準点かつ目標と比べて向きが下向きに違う場合
const SceneOrder DownTurn[] =
{
    {0, static_cast<int>(MoveSceneID::AltMove), ActionType::Move},
    {1, static_cast<int>(TurnSceneID::Turn180Right), ActionType::Turn}
};

// 目標ではない基準点かつ目標と比べて向きが上向きに違う場合
const SceneOrder UpTurn[] =
{
    {0, static_cast<int>(MoveSceneID::AltMove), ActionType::Move},
    {1, static_cast<int>(TurnSceneID::Turn180Right), ActionType::Turn}
};

// 目標ではない基準点を通過
const SceneOrder PassPoint[] =
{
    {0, static_cast<int>(MoveSceneID::PassPoint), ActionType::Move},
};


// 基準線からゲート方向へ90度旋回
const SceneOrder CourseTurn[] =
{
    {0, static_cast<int>(TurnSceneID::Turn90Right), ActionType::Turn},
    {1, static_cast<int>(TurnSceneID::Turn90Left),  ActionType::Turn}
};

// ゲート方向へ90度旋回
const SceneOrder GateTurn[] =
{
    {0, static_cast<int>(TurnSceneID::Turn90Left), ActionType::Turn},
    {1, static_cast<int>(TurnSceneID::Turn90Right),  ActionType::Turn}
};

// ゲート通過後の旋回
const SceneOrder GateCrossingTurn[] =
{
    {0, static_cast<int>(TurnSceneID::Turn90Left), ActionType::Turn},
    {1, static_cast<int>(TurnSceneID::Turn90Right), ActionType::Turn},
    {2, static_cast<int>(TurnSceneID::Turn180Right), ActionType::Turn}
};


// 次のエッジへ復帰するための旋回
const SceneOrder RejoinTurn[] =
{
    {0, static_cast<int>(TurnSceneID::Turn90Right), ActionType::Turn},
    {1, static_cast<int>(TurnSceneID::Turn90Left),  ActionType::Turn},
};

// 次のエッジへ復帰するための旋回
const SceneOrder BaseLineMove[] =
{
    {0, static_cast<int>(MoveSceneID::RejoinBaseLine), ActionType::Move},
};

// 次のエッジへ復帰するための旋回
const SceneOrder RejoinTurn2[] =
{
    {0, static_cast<int>(TurnSceneID::Turn45Right), ActionType::Turn},
    {1, static_cast<int>(TurnSceneID::Turn45Left),  ActionType::Turn},
};

// ゲート前まで移動
const SceneOrder EnterGate[] =
{
    {0, static_cast<int>(MoveSceneID::GatePosition1_5_10), ActionType::Move},
    {1, static_cast<int>(MoveSceneID::GatePosition2_6_11), ActionType::Move},
    {2, static_cast<int>(MoveSceneID::GatePosition3_7_12), ActionType::Move},
    {3, static_cast<int>(MoveSceneID::GatePosition4_8_13), ActionType::Move},
    {4, static_cast<int>(MoveSceneID::GatePosition9),      ActionType::Move},
};


// ゲート通過
const SceneOrder GateCrossing[] =
{
    {0, 9,  ActionType::Move},
    {1, 12, ActionType::Move},
};


// 基準点へ帰還
const SceneOrder ReturnPoint[] =
{
    {0, static_cast<int>(MoveSceneID::PositionReturn1_5_10), ActionType::Move},
    {1, static_cast<int>(MoveSceneID::PositionReturn2_6_11), ActionType::Move},
    {2, static_cast<int>(MoveSceneID::PositionReturn3_7_12), ActionType::Move},
    {3, static_cast<int>(MoveSceneID::PositionReturn4_8_13), ActionType::Move},
    {4, static_cast<int>(MoveSceneID::PositionReturn9), ActionType::Move},
};

const SceneOrder AltProc[] = 
{
    {0, static_cast<int>(LineTraceSceneID::LeftEdgeLineTrace),  ActionType::LineTrace},
    {1, static_cast<int>(MoveSceneID::AltMove),  ActionType::Move},
    {2, static_cast<int>(TurnSceneID::Turn180Right),  ActionType::Turn},
};

const SceneOrder GreenAltProc[] = 
{
    {0, static_cast<int>(LineTraceSceneID::GreenLeftEdgeLineTrace),  ActionType::LineTrace},
    {1, static_cast<int>(MoveSceneID::AltMove),  ActionType::Move},
    {2, static_cast<int>(TurnSceneID::Turn180Right),  ActionType::Turn},
};

// 停止
const SceneOrder stop[] =
{
    {0, 0, ActionType::Stop},
};

} // namespace


RallyStrategy::RallyStrategy(
    SceneManager& sceneManager)
    : mSceneManager(sceneManager)
{
}


void RallyStrategy::execute()
{
    Logger::printf("[Rally]ラリー開始\r\n");

    // ============================================================
    // 初期設定
    // ============================================================

    constexpr int LAP_COUNT = 3;

    // 最初は右エッジを使用
    // 周回をまたいでもエッジは引き継ぐ
    int nowEdgeIndex = RIGHT_EDGE_INDEX;

    // ゲートの数を配列情報から取得
    const int gatePositionCount =
        static_cast<int>(
            sizeof(gatePositions) /
            sizeof(gatePositions[0]));


    // ============================================================
    // 3周する
    // ============================================================

    for (int lap = 0; lap < LAP_COUNT; lap++)
    {
        Logger::printf(
            "[Rally] %d周目開始\r\n",
            lap + 1);


        // ========================================================
        // ゲートを順番に攻略
        // ========================================================

        for (int gateIndex = 0; gateIndex < gatePositionCount; gateIndex++)
        {
            const GatePosition& gate = gatePositions[gateIndex];

            Logger::printf(
                "[Rally]ゲート攻略開始 Gate=%d\r\n",
                gate.gatePositionNum);


            // ====================================================
            // 目標基準点を探す
            // ====================================================

            Color detectedPointColor = Color::Unknown;

            int pointSearchCount = 0;

            while (detectedPointColor != gate.pointColor)
            {
                if (pointSearchCount >= MAX_POINT_SEARCH_COUNT)
                {
                    Logger::printf(
                        "[Rally]基準点探索失敗 Gate=%d\r\n",
                        gate.gatePositionNum);

                    // finish();
                    return;
                }

                // ------------------------------------------------
                // 現在のエッジを使用して
                // 次の基準点までライントレース
                // ------------------------------------------------

                while(true)
                {
                    changeScene(&EnterPoint[nowEdgeIndex],0);

                    changeScene(stop,0);

                    // ------------------------------------------------
                    // 4色を順番に判定
                    // ------------------------------------------------

                    detectedPointColor = detectPointColor();

                    Logger::printf(
                        "[Rally]検知基準点=%d "
                        "目標基準点=%d\r\n",
                        static_cast<int>(detectedPointColor),
                        static_cast<int>(gate.pointColor));


                    // ------------------------------------------------
                    // 目標基準点なら中央まで移動
                    // それ以外なら通過
                    // ------------------------------------------------

                    if (detectedPointColor == gate.pointColor)
                    {
                        mOld_color = detectedPointColor;

                        if (changeScene(MovePointCenter,0))
                        {
                            // finish();
                            break;
                        }
                    }
                    else if(detectedPointColor == Color::Unknown)
                    {
                        continue;
                    }
                    else if(detectedPointColor > gate.pointColor && nowEdgeIndex == RIGHT_EDGE_INDEX)
                    {
                        changeScene(DownTurn,1);
                        nowEdgeIndex = LEFT_EDGE_INDEX;
                        continue;
                    }
                    else if(detectedPointColor < gate.pointColor && nowEdgeIndex == LEFT_EDGE_INDEX)
                    {
                        changeScene(UpTurn,1);
                        nowEdgeIndex = RIGHT_EDGE_INDEX;
                        continue;
                    }
                    else
                    {
                        if (changeScene(PassPoint,0))
                        {
                            // finish();
                            break;
                        }
                    }    
                }
                pointSearchCount++;
            }


            Logger::printf(
                "[Rally]目標基準点到達 Gate=%d\r\n",
                gate.gatePositionNum);


            // ====================================================
            // ゲートへ向かう
            // ====================================================

            // ゲートへ向かう前のエッジを保存
            const int gateApproachEdgeIndex = nowEdgeIndex;


            // ----------------------------------------------------
            // 基準線からゲート方向へ90度旋回
            // ----------------------------------------------------

            if (!changeScene(&CourseTurn[gateApproachEdgeIndex],0))
            {
                // finish();
                return;
            }


            // ====================================================
            // ゲート位置に応じてゲートへ進入
            // ====================================================

            const int gatePosition = gate.gatePositionNum;


            if (gatePosition <= 4 || gatePosition >= 10)
            {
                // --------------------------------------------
                // ゲート 1～4、10～13
                // --------------------------------------------

                int enterGateIndex;
                int turnDirection;

                if (gatePosition <= 4)
                {
                    enterGateIndex = gatePosition - 1;

                    turnDirection = UP_LEFT_TURN_INDEX;
                }
                else
                {
                    enterGateIndex = gatePosition - 10;

                    turnDirection = DOWN_RIGHT_TURN_INDEX;
                }

                if (enterGateIndex < 0 || enterGateIndex >= 5)
                {
                    Logger::printf(
                        "[Rally]ゲート番号不正=%d\r\n",
                        gatePosition);

                    // finish();
                    return;
                }


                // ------------------------------------------------
                // ゲート前まで移動
                // ------------------------------------------------

                if (!changeScene(&EnterGate[enterGateIndex],0))
                {
                    // finish();
                    return;
                }


                // ------------------------------------------------
                // ゲート通過方向へ旋回
                // ------------------------------------------------

                if (!changeScene(&GateTurn[turnDirection],0))
                {
                    // finish();
                    return;
                }


                // ------------------------------------------------
                // ゲートを通過
                // ------------------------------------------------

                if (!changeScene(GateCrossing,1))
                {
                    // finish();
                    return;
                }


                // ------------------------------------------------
                // 基準点方向へ旋回
                // ------------------------------------------------

                if (!changeScene(&GateCrossingTurn[turnDirection],0))
                {
                    // finish();
                    return;
                }
            }
            else
            {
                // --------------------------------------------
                // ゲート 5～9
                // --------------------------------------------

                const int enterGateIndex = gatePosition - 5;

                if (enterGateIndex < 0 || enterGateIndex >= 5)
                {
                    Logger::printf(
                        "[Rally]ゲート番号不正=%d\r\n",
                        gatePosition);

                    // finish();
                    return;
                }


                // ------------------------------------------------
                // ゲート通過
                // ------------------------------------------------

                if (!changeScene(&EnterGate[enterGateIndex],0))
                {
                    // finish();
                    return;
                }


                if (!changeScene(&GateCrossingTurn[VERTICAL_GATE_TURN_INDEX],0))
                {
                    // finish();
                    return;
                }
            }


            // ====================================================
            // 基準点へ帰還
            // ====================================================

            if (gatePosition <= 4 || gatePosition >= 10)
            {
                // --------------------------------------------
                // ゲート 1～4、10～13
                // --------------------------------------------

                int rejoinIndex;

                if (gatePosition <= 4)
                {
                    rejoinIndex = gatePosition - 1;
                }
                else
                {
                    rejoinIndex = gatePosition - 10;
                }

                if (rejoinIndex < 0 || rejoinIndex >= 5)
                {
                    Logger::printf(
                        "[Rally]ゲート番号不正=%d\r\n",
                        gatePosition);

                    // finish();
                    return;
                }
                if (!changeScene(&ReturnPoint[rejoinIndex],0))
            {
                // finish();
                return;
            }
            }
            else
            {
                // --------------------------------------------
                // ゲート 5～9
                // --------------------------------------------

                const int rejoinIndex = gatePosition - 5;

                if (rejoinIndex < 0 || rejoinIndex >= 5)
                {
                    Logger::printf(
                        "[Rally]ゲート番号不正=%d\r\n",
                        gatePosition);

                    // finish();
                    return;
                }
                if (!changeScene(&ReturnPoint[rejoinIndex],0))
                {
                    // finish();
                    return;
                }
            }

            changeScene(stop,0);

            // ====================================================
            // 次のゲートへ向かう
            // ====================================================

            // --------------------------------------------
            // 次のゲートがある場合
            //
            // 同じ周：
            //   Gate 3 → Gate 9
            //   Gate 9 → Gate 13
            //
            // 次の周：
            //   Gate 13 → Gate 3
            // --------------------------------------------

            const bool hasNextGate = (gateIndex + 1 < gatePositionCount);

            const bool hasNextLap = (lap + 1 < LAP_COUNT);

            // 最終周の最後のゲートなら終了
            if (!hasNextGate && !hasNextLap)
            {
                continue;
            }

            // ----------------------------------------------------
            // 帰還した基準点の色を4色判定
            // ----------------------------------------------------

            const Color returnedPointColor = mOld_color;
            //detectPointColor();

            // ----------------------------------------------------
            // 次に攻略するゲートを決定
            //
            // 同じ周の途中
            //   gateIndex + 1
            //
            // 周の最後
            //   次周の gateIndex 0
            // ----------------------------------------------------

            const int nextGateIndex =
                hasNextGate
                ? gateIndex + 1
                : 0;


            const Color nextPointColor = gatePositions[nextGateIndex].pointColor;

            Logger::printf(
                "[Rally]次のゲート=%d\r\n",
                gatePositions[
                    nextGateIndex].gatePositionNum);


            // ----------------------------------------------------
            // 次の基準点へ向かうエッジを決定
            // ----------------------------------------------------

            switch(getNextEdgeIndex(returnedPointColor,nextPointColor,nowEdgeIndex))
            {
                case 1:
                    //今の基準点と次の基準点が同じ時
                    mIsSameBasePoint = true;
                    changeScene(&RejoinTurn[1],0);
                    nowEdgeIndex = RIGHT_EDGE_INDEX;
                    break;
                case 2:
                {
                    //今の基準点と次の基準点が同じじゃない時
                    mIsSameBasePoint = false;
                    // ----------------------------------------------------
                    // 次の基準点方向へ旋回
                    //
                    // RIGHT_EDGE → 上方向
                    // LEFT_EDGE  → 下方向
                    // ----------------------------------------------------

                    const int rejoinTurnIndex =
                        nowEdgeIndex == RIGHT_EDGE_INDEX
                        ? UP_REJOIN_TURN_INDEX
                        : DOWN_REJOIN_TURN_INDEX;

                    changeScene(&RejoinTurn[rejoinTurnIndex],0);
                    break;
                }
                default:
                    Logger::printf(
                    "[Rally]次のエッジ決定失敗\r\n");
                    break;
            }

            Logger::printf(
                "[Rally]次のエッジ=%d\r\n",
                nowEdgeIndex);

            changeScene(BaseLineMove, 0);

            if(mIsSameBasePoint == true)
            {
                if(mOld_color == Color::Green)
                {
                    changeScene(GreenAltProc,2);    
                }
                else
                {
                    changeScene(AltProc,2);
                }
            }

            // ここでは nowEdgeIndex を初期化しない。
            //
            // 次の周でも現在のエッジを引き継ぐ。
        }
    }


    // ============================================================
    // ラリー終了
    // ============================================================

    Logger::printf(
        "[Rally]ラリー終了\r\n");

    // finish();
}


void RallyStrategy::update(
    int sceneId,
    bool result)
{
    // 状態遷移は後で実装
}


void RallyStrategy::updateNextScene()
{
    // 状態遷移は後で実装
}


void RallyStrategy::finish()
{
    //
}


// ============================================================
// 4色判定
// ============================================================
//
// ColorDetectScene
//
// Scene 0 → Yellow
// Scene 1 → Blue
// Scene 2 → Red
// Scene 3 → Green
//
// 各Sceneで「この色ですか？」を判定し、
// trueになったSceneから検出色を決定する。
// ============================================================

Color RallyStrategy::detectPointColor()
{
    for (int sceneId = 0;
         sceneId < COLOR_DETECT_SCENE_COUNT;
         sceneId++)
    {
        mSceneManager.setActionType(
            ActionType::ColorDetect);

        mSceneManager.setSceneID(
            sceneId);


        Logger::printf(
            "[Rally]色判定 SceneID=%d\r\n",
            sceneId);


        if (mSceneManager.SceneExecute())
        {
            switch (sceneId)
            {
            case YELLOW_COLOR_SCENE_ID:
                Logger::printf(
                    "[Rally]検出色=Green\r\n");

                return Color::Green;

            case BLUE_COLOR_SCENE_ID:
                Logger::printf(
                    "[Rally]検出色=Blue\r\n");

                return Color::Blue;

            case RED_COLOR_SCENE_ID:
                Logger::printf(
                    "[Rally]検出色=Red\r\n");

                return Color::Red;

            case GREEN_COLOR_SCENE_ID:
                Logger::printf(
                    "[Rally]検出色=Yellow\r\n");

                return Color::Yellow;

            default:
                break;
            }
        }
    }


    Logger::printf(
        "[Rally]色を検出できませんでした\r\n");

    return Color::Unknown;
}

// ============================================================
// シーン実行
// ============================================================

bool RallyStrategy::changeScene(
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


        Logger::printf(
            "[Rally]SceneID=%d\r\n",
            sceneOrderData.sceneId);


        if (!mSceneManager.SceneExecute())
        {
            Logger::printf(
                "[Rally]Scene実行失敗 ID=%d\r\n",
                sceneOrderData.sceneId);

            return false;
        }


        sceneNum++;


        if (sceneNum > maxSceneNum)
        {
            break;
        }
    }


    return true;
}