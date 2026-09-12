#pragma once

// コース設定: Left = 1、Right = -1。
// この値を変更して、全体を再ビルドしてください。
constexpr int COURSE_DIRECTION = 1;

static_assert(
    COURSE_DIRECTION == 1 || COURSE_DIRECTION == -1,
    "COURSE_DIRECTION must be 1 (Left) or -1 (Right).");

// シーンのエッジ・旋回角度は、常にLeftコース基準で定義します。
// SceneManagerが実行時にこの係数を一度だけ掛けます。
// 前後方向・距離・色・機体固有のモーター補正には掛けません。
