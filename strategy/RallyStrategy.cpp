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
bool getNextEdgeIndex(
    Color currentColor,
    Color nextColor,
    int& edgeIndex)
{
    const int currentOrder = getPointOrder(currentColor);

    const int nextOrder = getPointOrder(nextColor);

    if (currentOrder < 0 ||
        nextOrder < 0 ||
        currentOrder == nextOrder)
    {
        return false;
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

    return true;
}


struct GatePosition
{
    Color pointColor;
    int gatePositionNum;
};

//
Color getPointColor(int gatePosition)
{
    if (gatePosition >= 1 && gatePosition <= 8)
    {
        return Color::Green;
    }
    else if (gatePosition >= 9 && gatePosition <= 12)
    {
        return Color::Yellow;
    }
    else if (gatePosition >= 13 && gatePosition <= 16)
    {
        return Color::Red;
    }
    else
    {
        return Color::Blue;
    }
}


// ============================================================
// ラリー用シーン
// ============================================================

// 次の基準点までライントレース
const SceneOrder EnterPoint[] =
{
    {0, 28, ActionType::LineTrace},
    {0, 29, ActionType::LineTrace},
};


// 目標基準点の中央まで移動
const SceneOrder MovePointCenter[] =
{
    {0, 3, ActionType::Move},
};


// 目標ではない基準点を通過
const SceneOrder PassPoint[] =
{
    {0, 11, ActionType::Move},
};


// 基準線からゲート方向へ90度旋回
const SceneOrder GateTurn[] =
{
    {0, 2, ActionType::Turn},
    {1, 1, ActionType::Turn}
};


// ゲート通過後の旋回
const SceneOrder GateCrossingTurn[] =
{
    {0, 2, ActionType::Turn},
    {1, 1, ActionType::Turn},
    {2, 8, ActionType::Turn}
};


// 次のエッジへ復帰するための旋回
const SceneOrder RejoinTurn[] =
{
    {0, 5, ActionType::Turn},
    {1, 4, ActionType::Turn},
};


// ラインへ復帰
const SceneOrder RejoinMove[] =
{
    {0, 13, ActionType::Move},
};


// ゲート前まで移動
const SceneOrder EnterGate[] =
{
    {0, 4, ActionType::Move},
    {1, 5, ActionType::Move},
    {2, 6, ActionType::Move},
    {3, 7, ActionType::Move},
    {4, 8, ActionType::Move},
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
    {0, 10, ActionType::Move},
};

} // namespace


RallyStrategy::RallyStrategy(
    SceneManager& sceneManager)
    : mSceneManager(sceneManager)
{
}


void RallyStrategy::execute(int redGatePosition, int blueGatePosition, int yellowGatePosition)
{
    Logger::printf("[Rally]ラリー開始\r\n");

    // ============================================================
    // 初期設定
    // ============================================================

    constexpr int LAP_COUNT = 3;
    
    const Color POINT_COLOR = getPointColor(redGatePosition); 

    // 最初は右エッジを使用
    // 周回をまたいでもエッジは引き継ぐ
    int nowEdgeIndex = RIGHT_EDGE_INDEX;

    int NowLine

    // ============================================================
    // 3周する
    // ============================================================

    for (int lap = 0; lap < LAP_COUNT; lap++)
    {
        Logger::printf(
            "[Rally] %d周目開始\r\n",
            lap + 1);

            // ====================================================
            // 目標基準点を探す
            // ====================================================

            Color detectedPointColor = Color::Unknown;

            int pointSearchCount = 0;

            while (detectedPointColor != POINT_COLOR)
            {
                if (pointSearchCount >= MAX_POINT_SEARCH_COUNT)
                {
                    Logger::printf(
                        "[Rally]基準点探索失敗\r\n");

                    // finish();
                    return;
                }


                // ------------------------------------------------
                // 現在のエッジを使用して
                // 次の基準点までライントレース
                // ------------------------------------------------

                if (!changeScene(&EnterPoint[nowEdgeIndex],0))
                {
                    // finish();
                    return;
                }


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
                    if (!changeScene(MovePointCenter,0))
                    {
                        // finish();
                        return;
                    }
                }
                else
                {
                    if (!changeScene(PassPoint,0))
                    {
                        // finish();
                        return;
                    }
                }

                pointSearchCount++;
            }


            Logger::printf(
                "[Rally]目標基準点到達\r\n");


            // ====================================================
            // ゲートへ向かう
            // ====================================================

            // ゲートへ向かう前のエッジを保存
            const int gateApproachEdgeIndex = nowEdgeIndex;


            // ----------------------------------------------------
            // 基準線からゲート方向へ90度旋回
            // ----------------------------------------------------

            if (!changeScene(&GateTurn[gateApproachEdgeIndex],0))
            {
                // finish();
                return;
            }


            // ====================================================
            // ゲート位置に応じてゲートへ進入
            // ====================================================

            changeScene(&EnterGate[(redGatePosition - 1) % 4], 0);

            if (redGatePosition >= 5)
            {
                changeScene(&GateTurn[0], 0);
            }
            else
            {
                changeScene(&GateTurn[1], 0);
            }

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

            const Color returnedPointColor = detectPointColor();

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

            if (!getNextEdgeIndex(returnedPointColor,nextPointColor,nowEdgeIndex))
            {
                Logger::printf(
                    "[Rally]次のエッジ決定失敗\r\n");

                // finish();
                return;
            }


            Logger::printf(
                "[Rally]次のエッジ=%d\r\n",
                nowEdgeIndex);


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


            if (!changeScene(&RejoinTurn[rejoinTurnIndex],0))
            {
                // finish();
                return;
            }


            // ----------------------------------------------------
            // ラインを検知するまで前進
            // ----------------------------------------------------

            if (!changeScene(RejoinMove,0))
            {
                // finish();
                return;
            }


            // ここでは nowEdgeIndex を初期化しない。
            //
            // 次の周でも現在のエッジを引き継ぐ。
        
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
                    "[Rally]検出色=Yellow\r\n");

                return Color::Yellow;

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
                    "[Rally]検出色=Green\r\n");

                return Color::Green;

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