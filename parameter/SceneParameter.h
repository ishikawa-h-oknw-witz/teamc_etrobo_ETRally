#pragma once

#include "SceneManager.h"

enum class LapLineTraceSceneID
{
    Straight1 = 0, // Lap直線1
    Curve1_1  = 1, // Lapカーブ1-1
    Curve1_2  = 2, // Lapカーブ1-2
    Curve1_3  = 3, // Lapカーブ1-3
    Straight2 = 4, // Lap直線2
    Curve2_1  = 5, // Lapカーブ2-1
    Curve2_2  = 6, // Lapカーブ2-2
    Curve2_3  = 7, // Lapカーブ2-3
    Straight3 = 8, // Lap直線3
    Curve3    = 9, // Lapカーブ3
    SShape1   = 10, // Lap蛇行1
    SShape2   = 11, // Lap蛇行2
    Straight4 = 12, // Lap直線4
    SlowDown  = 13,  // Lap減速
    EnterCurve1 = 14, // ラップ後カーブ1
    EnterCurve2 = 15, // ラップ後カーブ2
};

enum class LapStopSceneID
{
    Finish = 0 // 停止
};

/* MARK:ラインID
 */
// ライントレース用シーンID
enum class LineTraceSceneID
{
    PassBlueLine         = 16, // Dlv行き青スルー
    EnterStraight1       = 17, // Dlv直線1
    EnterCurve3          = 18, // Dlvカーブ3

    ApproachYellowArea   = 19, // 黄エリア前まで
    ApproachBlueArea     = 20, // 青エリア前まで
    ApproachRedArea      = 21, // 赤エリア前まで

    ReturnFromYellowArea = 22, // Dlv黄から行きゲート前まで
    ReturnFromBlueArea   = 23, // Dlv青から行きゲート前まで
    ReturnFromRedArea    = 24, // Dlv赤から行きゲート前まで

    ReturnCurve1         = 25, // Dlv帰還カーブ1
    ReturnToBlue         = 26, // Dlv帰還青まで
    ReturnBlueHalfway    = 27, // Dlv青半分まで

    RightEdgeLineTrace = 28,   // ラリー基準点までの右エッジ走行用
    LeftEdgeLineTrace  = 29,    // ラリー基準点までの左エッジ走行用
    GreenLeftEdgeLineTrace  = 30,  //同一基準点が緑の場合 

    EnterGarageGreen = 31, //緑からガレージ
    EnterGarageYellow = 32, //黄からガレージ
    EnterGarageRed = 33, //赤からガレージ
    EnterGarageBlue = 34, //青からガレージ
    GrageLineTrace = 35,  //ガレージの前の線に復帰
};

/* MARK:移動シーンID
 */
// 移動用シーンID
enum class MoveSceneID
{
    MoveToBottlePosition =  2, // ボトル前まで移動
    MoveToDeliveryArea   =  0, // Dlvエリアまで
    ReturnToDeliveryLine =  1, // Dlv線まで帰還
    ReturnToBaseline     =  0, // Dlv基準線まで
    back                 =  19,// Dlvボトルを倒さないようにバック

    MoveToPointCenter    =  3, //目標基準点の中央まで
    PassPoint            = 11, //目標ではない基準点をスキップ
    RejoinBaseLine      = 13, //基準線に帰還

    GatePosition1_5_10   =  4, //ゲート通過番号1,5,10の移動
    GatePosition2_6_11   =  5, //ゲート通過番号2,6,11の移動
    GatePosition3_7_12   =  6, //ゲート通過番号3,7,12の移動
    GatePosition4_8_13   =  7, //ゲート通過番号4,8,13の移動
    GatePosition9        =  8, //ゲート通過番号9の移動

    PositionReturn1_5_10  =  14, //ゲート通過番号1,5,10の移動
    PositionReturn2_6_11  =  15, //ゲート通過番号2,6,11の移動
    PositionReturn3_7_12  =  16, //ゲート通過番号3,7,12の移動
    PositionReturn4_8_13  =  17, //ゲート通過番号4,8,13の移動
    PositionReturn9       =  18, //ゲート通過番号9の移動

    AltMove               =  20,

    RecoveryBottleDetectFront = 22,
    RecoveryBottleDetectBack = 23,
    MoveGarageLine = 24, //ガレージ線まで
    MoveInGarage = 25, //ガレージ中まで
    MoveException = 26, //ポールをよけるための例外移動
    EnterGarageGreen = 27, //緑からガレージ
    EnterGarageYellow = 28, //黄からガレージ
    EnterGarageRed = 29, //赤からガレージ
    EnterGarageBlue = 30, //青からガレージ
};

/* MARK:回転シーンID
 */
// 旋回用シーンID
enum class TurnSceneID
{
    Turn180Left  = 1,  // 左180°
    Turn180Right = 2,  // 右180°

    Turn150Left  = 3,  // 左150°
    Turn150Right = 4,  // 右150°

    Turn90Left   = 5,  // 左90°
    Turn90Right  = 6,  // 右90°

    Turn60Left   = 7,  // 左60°
    Turn60Right  = 8,  // 右60°

    Turn45Left   = 9,  // 左45°
    Turn45Right  = 10, // 右45°

    Turn30Left   = 11, // 左30°
    Turn30Right  = 12, // 右30°

    Turn15Left   = 13, // 左15°
    Turn15Right  = 14, // 右15°

    Turn10Left   = 15, // 左10°
};


// ボトル検知用シーンID
enum class BottleDetectSceneID
{
    DetectYellowBottle = 0, // 黄ボトル検知
    DetectBlueBottle   = 1, // 青ボトル検知
    DetectRedBottle    = 2  // 赤ボトル検知
};


// 停止用シーンID
enum class StopSceneID
{
    CheckCount = 0, // 回数確認用
    Finish     = 0,  // 終了
    stop = 0,
};

struct SceneOrder
{
    int sceneNum;
    int sceneId;
    ActionType actionType;
};