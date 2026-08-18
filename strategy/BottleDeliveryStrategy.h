#pragma once

#include "IChallengeStrategy.h"
#include "SceneManager.h"
#include "SceneParameter.h"
#include "ArmController.h"

class BottleDeliveryStrategy : public IChallengeStrategy
{
public:
    BottleDeliveryStrategy(
        SceneManager& sceneManager,
        ArmController& armController);

    void execute() override;
    void update(int sceneId, bool result) override;
    void updateNextScene() override;
    void finish() override;

private:
    SceneManager& mSceneManager;
    ArmController& mArmController;

    int mSkipCount;

    void changeScene(
        const SceneOrder sceneOrder[],
        int maxSceneNum);

    static const SceneOrder EnterBottle[];
    static const SceneOrder DetectBottleColor[];
    static const SceneOrder EnterZone[];
    static const SceneOrder MoveZone[];
    static const SceneOrder CarryZone[];
    static const SceneOrder ReturnZone[];
    static const SceneOrder EnterRally[];
};