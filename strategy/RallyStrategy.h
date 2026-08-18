#pragma once

#include "IChallengeStrategy.h"
#include "SceneManager.h"
#include "SceneParameter.h"

class RallyStrategy : public IChallengeStrategy
{
public:
    explicit RallyStrategy(
        SceneManager& sceneManager);

    void execute() override;
    void update(int sceneId, bool result) override;
    void updateNextScene() override;
    void finish() override;

private:
    SceneManager& mSceneManager;

    int mCurrentScene;
    bool mFinished;

};