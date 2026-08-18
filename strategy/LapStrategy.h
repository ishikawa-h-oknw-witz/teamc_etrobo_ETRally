#pragma once

#include "IChallengeStrategy.h"
#include "SceneManager.h"
#include "SceneParameter.h"

class LapStrategy : public IChallengeStrategy
{
public:
    LapStrategy(SceneManager& sceneManager);

    void execute() override;
    void update(int sceneId, bool result) override;
    void updateNextScene() override;
    void finish() override;

private:
    SceneManager& mSceneManager;

    void changeScene(
        const SceneOrder sceneOrder[],
        int maxSceneNum);

    static const SceneOrder LAP[];
};