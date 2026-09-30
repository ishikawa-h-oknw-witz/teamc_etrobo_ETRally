#pragma once

// コース設定
// Left = 1
// Right = -1

#define COURSE_LEFT
// #define COURSE_RIGHT

// シーンのエッジ・旋回角度は、常にLeftコース基準で定義します。
// SceneManagerが実行時にこの係数を一度だけ掛けます。
// 前後方向・距離・色・機体固有のモーター補正には掛けません。
