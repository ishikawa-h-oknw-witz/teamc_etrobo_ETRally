#pragma once

#include "IChallengeStrategy.h"
#include "SceneManager.h"
#include "SceneParameter.h"
#include "ArmController.h"

class RallyStrategy : public IChallengeStrategy
{
public:
    RallyStrategy(SceneManager& sceneManager,
                  ArmController& armController);

    void execute() override;
    void update(int sceneId, bool result) override;
    void updateNextScene() override;
    void finish() override;

private:
    SceneManager& mSceneManager;
    ArmController& mArmController;

    bool changeScene(
        const SceneOrder sceneOrder[],
        int maxSceneNum);

    Color detectPointColor();

    Color mOld_color = Color::Unknown;

    bool mIsSameBasePoint = false;
};