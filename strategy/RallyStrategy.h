#pragma once

#include "IChallengeStrategy.h"
#include "SceneManager.h"
#include "SceneParameter.h"

class RallyStrategy : public IChallengeStrategy
{
public:
    RallyStrategy(
        SceneManager& sceneManager,
        int redGatePosition,
        int blueGatePosition,
        int yellowGatePosition);

    void execute() override;
    void update(int sceneId, bool result) override;
    void updateNextScene() override;
    void finish() override;

private:
    SceneManager& mSceneManager;
    int mRedGatePosition;
    int mBlueGatePosition;
    int mYellowGatePosition;

    bool changeScene(
        const SceneOrder sceneOrder[],
        int maxSceneNum);

    Color detectPointColor();
};
