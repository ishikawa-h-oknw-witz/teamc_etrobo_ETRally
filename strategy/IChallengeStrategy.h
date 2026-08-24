#pragma once

class IChallengeStrategy
{
public:
    virtual void execute(int redGatePosition, int blueGatePosition, int yellowGatePosition) = 0;
    virtual void update(int sceneId, bool result) = 0;
    virtual void updateNextScene() = 0;
    virtual void finish() = 0;

    virtual ~IChallengeStrategy() = default;
};