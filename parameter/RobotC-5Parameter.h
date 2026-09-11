#pragma once

#include "SceneManager.h"

// ============================================================
// Robot C-5 Parameter
// ============================================================

// {シーンID, 台形制御パラメータ(開始速度、上限速度、終了速度、制御区間距離)、目標距離, 速度(今は使ってないメモ用), 走行エッジ, 終了色, 目標輝度, {Kp, Ki, Kd}}
const LineTraceScene lineTraceScenes[] =
{
    { 0,  {60.0f, 100.0f, 100.0f, 500.0f}, 500, 100, RunnerEdge::RightEdge, {Color::None}, CalibrationData::BlackWhiteCenter, {0.2f, 0.0f, 0.4f}}, // Lap直線1
    { 1,  {80.0f, 80.0f,  80.0f, 150.0f}, 150,  80, RunnerEdge::RightEdge, {Color::None}, CalibrationData::BlackWhiteCenter, {0.6f, 0.0f, 0.4f}}, // Lapカーブ1-1
    { 2,  {70.0f, 70.0f, 70.0f,  100.0f}, 100,  70, RunnerEdge::RightEdge, {Color::None}, CalibrationData::BlackWhiteCenter, {0.5f, 0.0f, 0.4f}}, // Lapカーブ1-2
    { 3,  {80.0f, 80.0f, 80.0f,  150.0f}, 150,  80, RunnerEdge::RightEdge, {Color::None}, CalibrationData::BlackWhiteCenter, {0.6f, 0.0f, 0.4f}}, // Lapカーブ1-3
    { 4,  {80.0f, 100.0f, 80.0f,  400.0f}, 400, 100, RunnerEdge::RightEdge, {Color::None}, CalibrationData::BlackWhiteCenter, {0.3f, 0.0f, 0.4f}}, // Lap直線2
    { 5,  {80.0f, 80.0f, 70.0f, 150.0f}, 150,  80, RunnerEdge::RightEdge, {Color::None}, CalibrationData::BlackWhiteCenter, {0.6f, 0.0f, 0.4f}}, // Lapカーブ2-1
    { 6,  {70.0f, 70.0f, 70.0f,  100.0f}, 100,  70, RunnerEdge::RightEdge, {Color::None}, CalibrationData::BlackWhiteCenter, {0.5f, 0.0f, 0.4f}}, // Lapカーブ2-2
    { 7,  {70.0f, 80.0f, 80.0f,  100.0f}, 100,  80, RunnerEdge::RightEdge, {Color::None}, CalibrationData::BlackWhiteCenter, {0.6f, 0.0f, 0.4f}}, // Lapカーブ2-3
    { 8,  {80.0f, 100.0f, 70.0f,  300.0f}, 300, 100, RunnerEdge::RightEdge, {Color::None}, CalibrationData::BlackWhiteCenter, {0.3f, 0.0f, 0.4f}}, // Lap直線3
    { 9,  {65.0f, 65.0f, 65.0f,  400.0f}, 400,  60, RunnerEdge::RightEdge, {Color::None}, CalibrationData::BlackWhiteCenter, {0.6f, 0.0f, 0.4f}}, // Lapカーブ3
    {10,  {65.0f, 90.0f, 70.0f,  900.0f}, 900, 100, RunnerEdge::RightEdge, {Color::None}, CalibrationData::BlackWhiteCenter, {0.6f, 0.0f, 0.4f}}, // Lap蛇行1
    {11,  {70.0f, 80.0f, 70.0f,  950.0f}, 950,  80, RunnerEdge::RightEdge, {Color::None}, CalibrationData::BlackWhiteCenter, {0.5f, 0.0f, 0.4f}}, // Lap蛇行2
    {12,  {80.0f, 100.0f, 70.0f,  850.0f}, 850, 100, RunnerEdge::RightEdge, {Color::None}, CalibrationData::BlackWhiteCenter, {0.4f, 0.0f, 0.4f}}, // Lap直線4
    {13,  {70.0f, 70.0f,  30.0f,  180.0f}, 180,  70, RunnerEdge::RightEdge, {Color::None}, CalibrationData::BlackWhiteCenter, {0.4f, 0.0f, 0.4f}}, // Lap減速

    {14,  {30.0f, 70.0f,  40.0f,  800.0f}, 800,  60, RunnerEdge::LeftEdge,  {Color::None}, CalibrationData::BlackWhiteCenter, {0.6f, 0.0f, 0.4f}}, // Dlvカーブ1
    {15,  {40.0f, 40.0f,  40.0f,  200.0f}, 200,  30, RunnerEdge::LeftEdge,  {Color::None}, CalibrationData::BlackWhiteCenter, {0.6f, 0.0f, 0.4f}}, // Dlvカーブ2
    {16,  {30.0f, 50.0f,  30.0f,  100.0f}, 100,  30, RunnerEdge::LeftEdge,  {Color::None}, CalibrationData::BlackWhiteCenter, {0.3f, 0.0f, 0.4f}}, // Dlv最初の青スルー
    {17,  {30.0f,100.0f,  50.0f,  1100.0f}, 1100,  70, RunnerEdge::LeftEdge,  {Color::None}, CalibrationData::BlackWhiteCenter, {0.3f, 0.0f, 0.4f}}, // Dlv直線1
    {18,  {40.0f, 40.0f,  40.0f,  200.0f}, 200,  30, RunnerEdge::LeftEdge,  {Color::None}, CalibrationData::BlackWhiteCenter, {0.8f, 0.0f, 0.4f}}, // Dlvカーブ3
    {19,  {30.0f, 80.0f,  50.0f,  350.0f}, 350,  70, RunnerEdge::LeftEdge,  {Color::None}, CalibrationData::BlackWhiteCenter, {0.3f, 0.0f, 0.4f}}, // 黄ボトル位置まで
    {20,  {30.0f, 100.0f, 50.0f,  600.0f}, 600,  70, RunnerEdge::LeftEdge,  {Color::None}, CalibrationData::BlackWhiteCenter, {0.3f, 0.0f, 0.4f}}, // 青ボトル位置まで
    {21,  {30.0f, 100.0f, 50.0f,  850.0f}, 850,  70, RunnerEdge::LeftEdge,  {Color::None}, CalibrationData::BlackWhiteCenter, {0.3f, 0.0f, 0.4f}}, // 赤ボトル位置まで

    {22,  {30.0f, 80.0f,  50.0f,  380.0f}, 380,  60, RunnerEdge::RightEdge, {Color::None}, CalibrationData::BlackWhiteCenter, {0.3f, 0.0f, 0.4f}}, // Dlv帰還直線 黄
    {23,  {30.0f, 80.0f, 50.0f,  630.0f}, 630,  60, RunnerEdge::RightEdge, {Color::None}, CalibrationData::BlackWhiteCenter, {0.3f, 0.0f, 0.4f}}, // Dlv帰還直線 青
    {24,  {30.0f, 100.0f, 50.0f,  880.0f}, 880,  60, RunnerEdge::RightEdge, {Color::None}, CalibrationData::BlackWhiteCenter, {0.3f, 0.0f, 0.4f}}, // Dlv帰還直線 赤
    {25,  {40.0f, 40.0f,  40.0f,  240.0f}, 240,  30, RunnerEdge::RightEdge, {Color::None}, CalibrationData::BlackWhiteCenter, {0.8f, 0.0f, 0.4f}}, // Dlv帰還カーブ1
    {26,  {30.0f, 100.0f, 30.0f, 1200.0f},1200, 100, RunnerEdge::RightEdge, {Color::None}, CalibrationData::BlackWhiteCenter, {0.3f, 0.0f, 0.4f}}, // Dlv基準線下まで
    {27,  {30.0f, 80.0f,  50.0f,  230.0f}, 230,  30, RunnerEdge::RightEdge, {Color::None}, CalibrationData::BlackWhiteCenter, {0.3f, 0.0f, 0.4f}}, // Dlv青半分まで、今使ってない

    {28,  {40.0f, 40.0f,  40.0f,   0.0f}, 0,  40, RunnerEdge::RightEdge, {Color::Green, Color::Yellow, Color::Red, Color::Blue},CalibrationData::BlackWhiteCenter, {0.5f, 0.0f, 0.6f}}, // Rly右エッジで次の色地点まで
    {29,  {40.0f, 40.0f,  40.0f,   0.0f}, 0,  40, RunnerEdge::LeftEdge,  {Color::Green, Color::Yellow, Color::Red, Color::Blue},CalibrationData::BlackWhiteCenter, {0.5f, 0.0f, 0.6f}}, // Rly左エッジで次の色地点まで

    {30,  {40.0f, 40.0f,  40.0f,  150.0f}, 150,  40, RunnerEdge::LeftEdge,  {Color::None}, CalibrationData::BlackWhiteCenter, {0.5f, 0.0f, 0.6f}}, // 今の基準点と次の基準点が同じかつその基準点の色が緑だった場合
};

const MoveScene moveScenes[] =
{
    { 0, Direction::front, {30.0f, 50.0f,  30.0f,   70.0f},  70, {Color::None}, {1.0f, 0.0f, 0.0f}}, // Dlvエリアまで
    { 1, Direction::back,  {40.0f, 60.0f,  30.0f,  180.0f}, 180, {Color::None}, {1.0f, 0.0f, 0.0f}}, // Dlv線まで帰還
    { 2, Direction::front, {30.0f,  40.0f,  30.0f,  60.0f},  60, {Color::None}, {1.0f, 0.0f, 0.0f}}, // Dlvボトルまで
    { 3, Direction::front, {30.0f,  30.0f,  30.0f,  70.0f}, 70, {Color::None}, {1.0f, 0.0f, 0.0f}}, // Rly基準点中央まで
    { 4, Direction::front, {70.0f,  80.0f,  30.0f, 270.0f}, 270, {Color::None}, {1.0f, 0.0f, 0.0f}}, // Rlyゲート前1
    { 5, Direction::front, {70.0f, 100.0f,  30.0f, 500.0f}, 500, {Color::None}, {1.0f, 0.0f, 0.0f}}, // Rlyゲート前2
    { 6, Direction::front, {70.0f, 100.0f,  30.0f, 750.0f}, 750, {Color::None}, {1.0f, 0.0f, 0.0f}}, // Rlyゲート前3
    { 7, Direction::front, {70.0f, 100.0f,  30.0f,1000.0f},1000, {Color::None}, {1.0f, 0.0f, 0.0f}}, // Rlyゲート前4
    { 8, Direction::front, {70.0f, 100.0f,  30.0f,1250.0f},1250, {Color::None}, {1.0f, 0.0f, 0.0f}}, // Rlyゲート前5
    { 9, Direction::front, {70.0f,  80.0f,  30.0f, 300.0f}, 300, {Color::None}, {1.0f, 0.0f, 0.0f}}, // Rlyゲートを通過
    {10, Direction::front, {80.0f,  80.0f,  80.0f,   0.0f},   0, {Color::Green, Color::Yellow, Color::Red, Color::Blue},{1.0f, 0.0f, 0.0f}}, //基準点帰還
    {11, Direction::front, {30.0f,  50.0f,  50.0f, 100.0f}, 100, {Color::None}, {1.0f, 0.0f, 0.0f}}, // Rly目標外の基準点を通過
    {12, Direction::back,  {30.0f,  70.0f,  30.0f, 280.0f}, 280, {Color::None}, {1.0f, 0.0f, 0.0f}}, // Rlyゲートから後退して帰還
    {13, Direction::front, {30.0f,  40.0f,  30.0f, 50.0f}, 50, {Color::None}, {1.0f, 0.0f, 0.0f}}, // Rly帰還旋回後にラインまで直進
    {14, Direction::front, {70.0f,  80.0f,  30.0f, 270.0f}, 270, {Color::None}, {1.0f, 0.0f, 0.0f}}, // 帰還1
    {15, Direction::front, {70.0f, 100.0f,  30.0f, 500.0f}, 500, {Color::None}, {1.0f, 0.0f, 0.0f}}, // 帰還2
    {16, Direction::front, {70.0f, 100.0f,  30.0f, 750.0f}, 750, {Color::None}, {1.0f, 0.0f, 0.0f}}, // 帰還3
    {17, Direction::front, {70.0f, 100.0f,  30.0f, 1000.0f}, 1000, {Color::None}, {1.0f, 0.0f, 0.0f}}, // 帰還4
    {18, Direction::front, {70.0f, 100.0f,  30.0f,1250.0f},1250, {Color::None}, {1.0f, 0.0f, 0.0f}}, // 帰還5
    {19, Direction::back, {30.0f, 30.0f,   30.0f,  50.0f},50, {Color::None}, {1.0f, 0.0f, 0.0f}}, // ボトルデリバリー用バック
    {20, Direction::front, {40.0f, 40.0f,  40.0f, 60.0f},60, {Color::None}, {1.0f, 0.0f, 0.0f}}, // 代替処理用move
    {21, Direction::front, {30.0f, 80.0f,  30.0f, 1250.0f},1250, {Color::None}, {1.0f, 0.0f, 0.0f}}, // 実験用move
};

const TurnScene turnScenes[] =
{
    {0,   0, {1.0f, 0.0f, 0.0f}}, //正面
    {1,  90, {1.0f, 0.0f, 0.0f}}, //R90°
    {2, -90, {1.0f, 0.0f, 0.0f}}, //L90°
    {3, -60, {1.0f, 0.0f, 0.0f}}, //L60°
    {4, -30, {1.0f, 0.0f, 0.0f}}, //L30°
    {5,  30, {1.0f, 0.0f, 0.0f}}, //R30°
    {6,-150, {1.0f, 0.0f, 0.0f}}, //L150°
    {7, 150, {1.0f, 0.0f, 0.0f}}, //R150°
    {8, 180, {1.0f, 0.0f, 0.0f}}, //後ろ
    {9, -15, {1.0f, 0.0f, 0.0f}}, //左15°
    {10, 45, {1.0f, 0.0f, 0.0f}}, //右45°
    {11,-45, {1.0f, 0.0f, 0.0f}},  //左45°
    {12, 60, {1.0f, 0.0f, 0.0f}}, //右60°
    {13,-60, {1.0f, 0.0f, 0.0f}},  //左60°
    {14,-180, {1.0f, 0.0f, 0.0f}}, //後ろ
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