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
    SlowDown  = 13  // Lap減速
};

enum class LapStopSceneID
{
    Finish = 0 // 停止
};

// ライントレース用シーンID
enum class LineTraceSceneID
{
    EnterCurve1          = 14, // Dlvカーブ1
    EnterCurve2          = 15, // Dlvカーブ2
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
    ReturnBlueHalfway    = 27  // Dlv青半分まで
};


// 移動用シーンID
enum class MoveSceneID
{
    MoveToBottlePosition = 2, // ボトル前まで移動
    MoveToDeliveryArea   = 0, // Dlvエリアまで
    ReturnToDeliveryLine = 1, // Dlv線まで帰還
    ReturnToBaseline     = 0  // Dlv基準線まで
};


// 旋回用シーンID
enum class TurnSceneID
{
    AdjustEnterAngle = 9, // エンターゾーン進入時の角度調整
    Turn90Right      = 1, // 右に90°回転
    TurnToRally      = 0  // Dlv右に90°回転
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
    Finish     = 0  // 終了
};

struct SceneOrder
{
    int sceneNum;
    int sceneId;
    ActionType actionType;
};