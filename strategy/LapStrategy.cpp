#include "LapStrategy.h"

const SceneOrder LapStrategy::LAP[] =
{
    { 0, static_cast<int>(LapLineTraceSceneID::Straight1), ActionType::LineTrace},
    { 1, static_cast<int>(LapLineTraceSceneID::Curve1_1),  ActionType::LineTrace},
    { 2, static_cast<int>(LapLineTraceSceneID::Curve1_2),  ActionType::LineTrace},
    { 3, static_cast<int>(LapLineTraceSceneID::Curve1_3),  ActionType::LineTrace},
    { 4, static_cast<int>(LapLineTraceSceneID::Straight2), ActionType::LineTrace},
    { 5, static_cast<int>(LapLineTraceSceneID::Curve2_1),  ActionType::LineTrace},
    { 6, static_cast<int>(LapLineTraceSceneID::Curve2_2),  ActionType::LineTrace},
    { 7, static_cast<int>(LapLineTraceSceneID::Curve2_3),  ActionType::LineTrace},
    { 8, static_cast<int>(LapLineTraceSceneID::Straight3), ActionType::LineTrace},
    { 9, static_cast<int>(LapLineTraceSceneID::Curve3),    ActionType::LineTrace},
    {10, static_cast<int>(LapLineTraceSceneID::SShape1),   ActionType::LineTrace},
    {11, static_cast<int>(LapLineTraceSceneID::SShape2),   ActionType::LineTrace},
    {12, static_cast<int>(LapLineTraceSceneID::Straight4), ActionType::LineTrace},
    {13, static_cast<int>(LapLineTraceSceneID::SlowDown),  ActionType::LineTrace},
    {14, static_cast<int>(LapStopSceneID::Finish),          ActionType::Stop}
};

LapStrategy::LapStrategy(
    SceneManager& sceneManager)
    : mSceneManager(sceneManager)
{
}

void LapStrategy::execute()
{
    changeScene(LAP, 14);
}

void LapStrategy::update(
    int sceneId,
    bool result)
{
    // 攻略状態を更新
}

void LapStrategy::updateNextScene()
{
    // 次のシーンIDを更新
}

void LapStrategy::finish()
{
    // ラップ攻略終了処理
}

void LapStrategy::changeScene(
    const SceneOrder sceneOrder[],
    int maxSceneNum)
{
    int sceneNum = 0;

    while (true)
    {
        const SceneOrder& sceneOrderData = sceneOrder[sceneNum];

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