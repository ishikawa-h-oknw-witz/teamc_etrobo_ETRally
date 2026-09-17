#pragma once

#include "SceneManager.h"

// ============================================================
// Robot B-5 Parameter
// ============================================================

// {シーンID, 台形制御パラメータ(開始速度、上限速度、終了速度、制御区間距離)、目標距離, 速度(今は使ってないメモ用), 走行エッジ, 終了色, 目標輝度, {Kp, Ki, Kd}}
const LineTraceScene lineTraceScenes[] =
{
    { 0,  {80.0f, 100.0f, 100.0f, 500.0f}, 500, 100, RunnerEdge::RightEdge, {Color::None}, CalibrationData::BlackWhiteCenter, {0.2f, 0.0f, 0.4f}}, // Lap直線1
    { 1,  {80.0f, 80.0f,  80.0f, 150.0f}, 150,  80, RunnerEdge::RightEdge, {Color::None}, CalibrationData::BlackWhiteCenter, {0.6f, 0.0f, 0.4f}}, // Lapカーブ1-1
    { 2,  {70.0f, 70.0f, 70.0f,  100.0f}, 100,  70, RunnerEdge::RightEdge, {Color::None}, CalibrationData::BlackWhiteCenter, {0.5f, 0.0f, 0.4f}}, // Lapカーブ1-2
    { 3,  {80.0f, 80.0f, 80.0f,  150.0f}, 150,  80, RunnerEdge::RightEdge, {Color::None}, CalibrationData::BlackWhiteCenter, {0.6f, 0.0f, 0.4f}}, // Lapカーブ1-3
    { 4,  {80.0f, 100.0f, 80.0f,  400.0f}, 400, 100, RunnerEdge::RightEdge, {Color::None}, CalibrationData::BlackWhiteCenter, {0.3f, 0.0f, 0.4f}}, // Lap直線2
    { 5,  {80.0f, 80.0f, 70.0f, 150.0f}, 150,  80, RunnerEdge::RightEdge, {Color::None}, CalibrationData::BlackWhiteCenter, {0.6f, 0.0f, 0.4f}}, // Lapカーブ2-1
    { 6,  {70.0f, 70.0f, 70.0f,  100.0f}, 100,  70, RunnerEdge::RightEdge, {Color::None}, CalibrationData::BlackWhiteCenter, {0.5f, 0.0f, 0.4f}}, // Lapカーブ2-2
    { 7,  {70.0f, 80.0f, 80.0f,  100.0f}, 100,  80, RunnerEdge::RightEdge, {Color::None}, CalibrationData::BlackWhiteCenter, {0.6f, 0.0f, 0.4f}}, // Lapカーブ2-3
    { 8,  {80.0f, 100.0f, 70.0f,  300.0f}, 300, 100, RunnerEdge::RightEdge, {Color::None}, CalibrationData::BlackWhiteCenter, {0.3f, 0.0f, 0.4f}}, // Lap直線3
    { 9,  {70.0f, 70.0f, 70.0f,  400.0f}, 400,  60, RunnerEdge::RightEdge, {Color::None}, CalibrationData::BlackWhiteCenter, {0.4f, 0.0f, 0.6f}}, // Lapカーブ3
    {10,  {70.0f, 100.0f, 80.0f,  900.0f}, 900, 100, RunnerEdge::RightEdge, {Color::None}, CalibrationData::BlackWhiteCenter, {0.4f, 0.0f, 0.6f}}, // Lap蛇行1
    {11,  {80.0f, 80.0f, 80.0f,  900.0f}, 900,  80, RunnerEdge::RightEdge, {Color::None}, CalibrationData::BlackWhiteCenter, {0.5f, 0.0f, 0.4f}}, // Lap蛇行2
    {12,  {80.0f, 100.0f, 70.0f,  900.0f}, 900, 100, RunnerEdge::RightEdge, {Color::None}, CalibrationData::BlackWhiteCenter, {0.4f, 0.0f, 0.4f}}, // Lap直線4
    {13,  {70.0f, 70.0f,  30.0f,  280.0f}, 280,  70, RunnerEdge::RightEdge, {Color::None}, CalibrationData::BlackWhiteCenter, {0.4f, 0.0f, 0.4f}}, // Lap減速
    {14,  {30.0f, 60.0f,  40.0f,  600.0f}, 600,  60, RunnerEdge::LeftEdge,  {Color::None}, CalibrationData::BlackWhiteCenter, {0.5f, 0.0f, 0.5f}}, // ラップ後カーブ1
    {15,  {40.0f, 40.0f,  40.0f,  400.0f}, 400,  30, RunnerEdge::LeftEdge,  {Color::None}, CalibrationData::BlackWhiteCenter, {0.6f, 0.0f, 0.4f}}, // ラップ後カーブ2

    {16,  {40.0f, 40.0f,  30.0f,  100.0f}, 100,  30, RunnerEdge::LeftEdge,  {Color::None}, CalibrationData::BlackWhiteCenter, {0.3f, 0.0f, 0.4f}}, // Dlv最初の青スルー
    {17,  {30.0f,100.0f,  50.0f,  1200.0f}, 1200,  70, RunnerEdge::LeftEdge,  {Color::None}, CalibrationData::BlackWhiteCenter, {0.4f, 0.0f, 0.6f}}, // Dlv直線1
    {18,  {30.0f, 30.0f,  30.0f,  200.0f}, 200,  30, RunnerEdge::LeftEdge,  {Color::None}, CalibrationData::BlackWhiteCenter, {0.6f, 0.0f, 0.4f}}, // Dlvカーブ3
    {19,  {30.0f, 80.0f,  50.0f,  300.0f}, 300,  70, RunnerEdge::LeftEdge,  {Color::None}, CalibrationData::BlackWhiteCenter, {0.4f, 0.0f, 0.6f}}, // 黄ボトル位置まで
    {20,  {30.0f, 100.0f, 50.0f,  550.0f}, 550,  70, RunnerEdge::LeftEdge,  {Color::None}, CalibrationData::BlackWhiteCenter, {0.4f, 0.0f, 0.6f}}, // 青ボトル位置まで
    {21,  {30.0f, 100.0f, 50.0f,  850.0f}, 850,  70, RunnerEdge::LeftEdge,  {Color::None}, CalibrationData::BlackWhiteCenter, {0.4f, 0.0f, 0.6f}}, // 赤ボトル位置まで

    {22,  {30.0f, 80.0f,  50.0f,  380.0f}, 380,  60, RunnerEdge::RightEdge, {Color::None}, CalibrationData::BlackWhiteCenter, {0.4f, 0.0f, 0.6f}}, // Dlv帰還直線 黄
    {23,  {30.0f, 80.0f, 50.0f,  630.0f}, 630,  60, RunnerEdge::RightEdge, {Color::None}, CalibrationData::BlackWhiteCenter, {0.4f, 0.0f, 0.6f}}, // Dlv帰還直線 青
    {24,  {30.0f, 100.0f, 50.0f,  930.0f}, 930,  60, RunnerEdge::RightEdge, {Color::None}, CalibrationData::BlackWhiteCenter, {0.4f, 0.0f, 0.6f}}, // Dlv帰還直線 赤
    {25,  {30.0f, 30.0f,  30.0f,  150.0f}, 150,  30, RunnerEdge::RightEdge, {Color::None}, CalibrationData::BlackWhiteCenter, {0.6f, 0.0f, 0.4f}}, // Dlv帰還カーブ1
    {26,  {30.0f, 100.0f, 30.0f, 1200.0f},   0, 100, RunnerEdge::RightEdge, {Color::Blue}, CalibrationData::BlackWhiteCenter, {0.4f, 0.0f, 0.6f}}, // Dlv基準線下まで
    {27,  {40.0f, 40.0f,  30.0f,  40.0f}, 40,  30, RunnerEdge::RightEdge, {Color::None}, CalibrationData::BlackWhiteCenter, {0.3f, 0.0f, 0.4f}}, // Dlv青半分まで

    {28,  {35.0f, 35.0f,  35.0f,   0.0f}, 0,  40, RunnerEdge::RightEdge, {Color::Green, Color::Yellow, Color::Red, Color::Blue},CalibrationData::BlackWhiteCenter, {0.3f, 0.0f, 0.5f}}, // Rly右エッジで次の色地点まで
    {29,  {35.0f, 35.0f,  35.0f,   0.0f}, 0,  40, RunnerEdge::LeftEdge,  {Color::Green, Color::Yellow, Color::Red, Color::Blue},CalibrationData::BlackWhiteCenter, {0.3f, 0.0f, 0.5f}}, // Rly左エッジで次の色地点まで

    {30,  {40.0f, 40.0f,  40.0f,  130.0f}, 130,  40, RunnerEdge::LeftEdge,  {Color::None}, CalibrationData::BlackWhiteCenter, {0.3f, 0.0f, 0.5f}}, // 今の基準点と次の基準点が同じかつその基準点の色が緑だった場合

    /*現在は使っていない。ガレージをライントレースで成立させたいときに使用*/
    {31,  {30.0f, 60.0f, 50.0f, 250.0f}, 250, 30, RunnerEdge::RightEdge, {Color::None}, CalibrationData::BlackWhiteCenter, {0.3f, 0.0f, 0.4f}}, //緑からガレージ
    {32,  {30.0f, 60.0f, 50.0f, 250.0f}, 250, 30, RunnerEdge::RightEdge, {Color::None}, CalibrationData::BlackWhiteCenter, {0.3f, 0.0f, 0.4f}}, //黄からガレージ
    {33,  {30.0f, 60.0f, 50.0f, 700.0f}, 700, 30, RunnerEdge::RightEdge, {Color::None}, CalibrationData::BlackWhiteCenter, {0.3f, 0.0f, 0.4f}}, //赤からガレージ
    {34,  {30.0f, 60.0f, 50.0f, 950.0f}, 950, 30, RunnerEdge::RightEdge, {Color::None}, CalibrationData::BlackWhiteCenter, {0.3f, 0.0f, 0.4f}}, //青からガレージ
    {35,  {30.0f, 50.0f, 50.0f,   0.0f},   0, 30, RunnerEdge::RightEdge, {Color::White}, CalibrationData::BlackWhiteCenter, {0.3f, 0.0f, 0.4f}}, //ガレージ中に移動
};

const MoveScene moveScenes[] =
{
    { 0, Direction::front, {30.0f, 50.0f,  30.0f,   70.0f},  70, {Color::None}, {2.0f, 0.0f, 0.0f}}, // Dlvエリアまで
    { 1, Direction::back,  {30.0f, 60.0f,  30.0f,  100.0f}, 100, {Color::None}, {2.0f, 0.0f, 0.0f}}, // Dlv線まで帰還
    { 2, Direction::front, {40.0f,  40.0f,  40.0f,  60.0f},  60, {Color::None}, {2.0f, 0.0f, 0.0f}}, // Dlvボトルまで
    { 3, Direction::front, {30.0f,  30.0f,  30.0f, 70.0f}, 70, {Color::None}, {3.0f, 0.0f, 0.0f}}, // Rly基準点中央まで
    { 4, Direction::front, {70.0f,  80.0f,  30.0f, 250.0f}, 250, {Color::None}, {3.0f, 0.0f, 0.0f}}, // Rlyゲート前1
    { 5, Direction::front, {70.0f, 100.0f,  30.0f, 500.0f}, 500, {Color::None}, {3.0f, 0.0f, 0.0f}}, // Rlyゲート前2
    { 6, Direction::front, {70.0f, 100.0f,  30.0f, 750.0f}, 750, {Color::None}, {3.0f, 0.0f, 0.0f}}, // Rlyゲート前3
    { 7, Direction::front, {70.0f, 100.0f,  30.0f,1000.0f},1000, {Color::None}, {3.0f, 0.0f, 0.0f}}, // Rlyゲート前4
    { 8, Direction::front, {70.0f, 100.0f,  30.0f,1250.0f},1250, {Color::None}, {3.0f, 0.0f, 0.0f}}, // Rlyゲート前5
    { 9, Direction::front, {70.0f,  80.0f,  30.0f, 300.0f}, 300, {Color::None}, {3.0f, 0.0f, 0.0f}}, // Rlyゲートを通過
    {10, Direction::front, {80.0f,  80.0f,  80.0f,   0.0f},   0, {Color::Green, Color::Yellow, Color::Red, Color::Blue},{2.0f, 0.0f, 0.0f}}, //基準点帰還
    {11, Direction::front, {30.0f,  50.0f,  50.0f, 100.0f}, 100, {Color::None}, {3.0f, 0.0f, 0.0f}}, // Rly目標外の基準点を通過
    {12, Direction::back,  {30.0f,  70.0f,  30.0f, 300.0f}, 300, {Color::None}, {3.0f, 0.0f, 0.0f}}, // Rlyゲートから後退して帰還
    {13, Direction::front, {30.0f,  40.0f,  30.0f, 50.0f}, 50, {Color::None}, {3.0f, 0.0f, 0.0f}}, // Rly帰還旋回後にラインまで直進
    {14, Direction::front, {70.0f,  80.0f,  30.0f, 250.0f}, 250, {Color::None}, {3.0f, 0.0f, 0.0f}}, // 帰還1
    {15, Direction::front, {70.0f, 100.0f,  30.0f, 500.0f}, 500, {Color::None}, {3.0f, 0.0f, 0.0f}}, // 帰還2
    {16, Direction::front, {70.0f, 100.0f,  30.0f, 750.0f}, 750, {Color::None}, {3.0f, 0.0f, 0.0f}}, // 帰還3
    {17, Direction::front, {70.0f, 100.0f,  30.0f, 1000.0f}, 1000, {Color::None}, {3.0f, 0.0f, 0.0f}}, // 帰還4
    {18, Direction::front, {70.0f, 100.0f,  30.0f,1250.0f},1250, {Color::None}, {3.0f, 0.0f, 0.0f}}, // 帰還5
    {19, Direction::back,  {40.0f, 40.0f,   40.0f,  50.0f},50, {Color::None}, {3.0f, 0.0f, 0.0f}}, // ボトルデリバリー用バック
    {20, Direction::front, {40.0f, 40.0f,  40.0f, 60.0f},60, {Color::None}, {3.0f, 0.0f, 0.0f}}, // 代替処理用move
    {21, Direction::front, {30.0f, 80.0f,  30.0f, 1250.0f},1250, {Color::None}, {3.0f, 0.0f, 0.0f}}, // 実験用move
    {22, Direction::front, {40.0f,  40.0f,  40.0f,  20.0f},  20, {Color::None}, {2.0f, 0.0f, 0.0f}}, // ボトルデリバリー検知失敗時復帰用
    {23, Direction::back,  {40.0f, 40.0f,   40.0f,  30.0f}, 30, {Color::None}, {2.0f, 0.0f, 0.0f}}, // ボトルデリバリー検知失敗時用バック
    {24, Direction::front, {30.0f, 80.0f, 30.0f, 600.0f}, 600, {Color::None}, {2.0f, 0.0f, 0.0f}}, //ガレージ線まで
    {25, Direction::front, {30.0f, 50.0f, 30.0f, 150.0f}, 150, {Color::None}, {2.0f, 0.0f, 0.0f}}, //ガレージ中まで
    {26, Direction::front, {50.0f, 50.0f, 30.0f, 470.0f}, 470, {Color::None}, {2.0f, 0.0f, 0.0f}}, //土俵前まで
    {27, Direction::front, {40.0f, 50.0f,50.0f, 220.0f}, 220, {Color::None}, {2.0f, 0.0f, 0.0f}}, //押し出し
    {28, Direction::back, {30.0f, 50.0f, 50.0f, 170.0f}, 170, {Color::None}, {2.0f, 0.0f, 0.0f}}, //押し出し
    {29, Direction::front, {70.0f,100.0f, 80.0f,1300.0f}, 1300, {Color::None}, {2.0f, 0.0f, 0.0f}}, //土俵前まで
    {30, Direction::front, {50.0f,100.0f, 50.0f, 300.0f}, 300, {Color::None}, {2.0f, 0.0f, 0.0f}}, //押し出し
    {31, Direction::back, {30.0f,60.0f, 40.0f, 70.0f}, 70, {Color::None}, {3.0f, 0.0f, 0.0f}}, //最後の目標基準点が青だった時の例外処理用
};

const TurnScene turnScenes[] =
{
    {0,    0, false, false, {1.0f, 0.0f, 0.0f}}, // 正面

    {1,  -180, false, false, {1.0f, 0.0f, 0.0f}}, // 左180°
    {2,   180, false, false, {1.0f, 0.0f, 0.0f}}, // 右180°

    {3,  -150, false, false, {1.0f, 0.0f, 0.0f}}, // 左150°
    {4,   150, false, false, {1.0f, 0.0f, 0.0f}}, // 右150°

    {5,   -90, false, false, {1.0f, 0.0f, 0.0f}}, // 左90°
    {6,    90, false, false, {1.0f, 0.0f, 0.0f}}, // 右90°

    {7,   -60, false, false, {1.0f, 0.0f, 0.0f}}, // 左60°
    {8,    60, false, false, {1.0f, 0.0f, 0.0f}}, // 右60°

    {9,   -45, false, false, {1.0f, 0.0f, 0.0f}}, // 左45°
    {10,   45, false, false, {1.0f, 0.0f, 0.0f}}, // 右45°

    {11,  -30, false, false, {1.0f, 0.0f, 0.0f}}, // 左30°
    {12,   30, false, false, {1.0f, 0.0f, 0.0f}}, // 右30°

    {13,  -15, false, false, {1.0f, 0.0f, 0.0f}}, // 左15°
    {14,   15, false, false, {1.0f, 0.0f, 0.0f}}, // 右15°

    {15,  -10, false, false, {1.0f, 0.0f, 0.0f}}, // 左10°

    {16,-360, true, false, {1.0f, 0.0f, 0.0f}}, //測距1
    {17, 0, false, true, {1.0f, 0.0f, 0.0f}} //測距1
};

const BottleDetectScene bottleDetectScenes[] =
{
    {0, {Color::Yellow}}, // 黄ボトル検知
    {1, {Color::Blue}},   // 青ボトル検知
    {2, {Color::Red}}     // 赤ボトル検知
};

const ColorDetectScene colorDetectScenes[] =
{
    {0, {Color::Green} }, //黄検知
    {1, {Color::Blue}  }, //青検知
    {2, {Color::Red}   }, //赤検知
    {3, {Color::Yellow}}  //緑検知
};