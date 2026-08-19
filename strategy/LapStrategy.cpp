#include "LapStrategy.h"

const SceneOrder LapStrategy::LAP[] =
{
    { 0, static_cast<int>(LapLineTraceSceneID::Straight1), ActionType::LineTrace},  //カーブ1までの直線
    { 1, static_cast<int>(LapLineTraceSceneID::Curve1_1),  ActionType::LineTrace},  //カーブ1始まり
    { 2, static_cast<int>(LapLineTraceSceneID::Curve1_2),  ActionType::LineTrace},  //カーブ1真ん中
    { 3, static_cast<int>(LapLineTraceSceneID::Curve1_3),  ActionType::LineTrace},  //カーブ1終わり
    { 4, static_cast<int>(LapLineTraceSceneID::Straight2), ActionType::LineTrace},  //カーブ2までの直線
    { 5, static_cast<int>(LapLineTraceSceneID::Curve2_1),  ActionType::LineTrace},  //カーブ2始まり
    { 6, static_cast<int>(LapLineTraceSceneID::Curve2_2),  ActionType::LineTrace},  //カーブ2真ん中
    { 7, static_cast<int>(LapLineTraceSceneID::Curve2_3),  ActionType::LineTrace},  //カーブ2終わり
    { 8, static_cast<int>(LapLineTraceSceneID::Straight3), ActionType::LineTrace},  //カーブ3までの直線
    { 9, static_cast<int>(LapLineTraceSceneID::Curve3),    ActionType::LineTrace},  //カーブ3
    {10, static_cast<int>(LapLineTraceSceneID::SShape1),   ActionType::LineTrace},  //蛇行1
    {11, static_cast<int>(LapLineTraceSceneID::SShape2),   ActionType::LineTrace},  //蛇行2
    {12, static_cast<int>(LapLineTraceSceneID::Straight4), ActionType::LineTrace},  //最後の直線
    {13, static_cast<int>(LapLineTraceSceneID::SlowDown),  ActionType::LineTrace},  //ボトル前の減速
    {14, static_cast<int>(LapStopSceneID::Finish),          ActionType::Stop}       //ボトル前で停止
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