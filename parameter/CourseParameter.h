#pragma once

// 起動時のコース設定: Left = 1、Right = -1。
// スタート待ち中にHubの左・右ボタンで切り替えられます。
constexpr int DEFAULT_COURSE_DIRECTION = 1;

static_assert(
    DEFAULT_COURSE_DIRECTION == 1 || DEFAULT_COURSE_DIRECTION == -1,
    "DEFAULT_COURSE_DIRECTION must be 1 (Left) or -1 (Right).");

// 実際に走るコース。app.cppで1個だけ定義し、SceneManagerと共有します。
// ヘッダーにstaticで定義するとファイルごとに別の変数になるため、externにします。
extern int COURSE_DIRECTION;

// シーンのエッジ・旋回角度は、常にLeftコース基準で定義します。
// SceneManagerが実行時にこの係数を一度だけ掛けます。
// 前後方向・距離・色・機体固有のモーター補正には掛けません。
