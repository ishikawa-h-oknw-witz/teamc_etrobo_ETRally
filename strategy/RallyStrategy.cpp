#include "RallyStrategy.h"

RallyStrategy::RallyStrategy(
    SceneManager& sceneManager)
    : mSceneManager(sceneManager),
      mCurrentScene(0),
      mFinished(false)
{
}


void RallyStrategy::execute()
{
    //
}


void RallyStrategy::update(
    int sceneId,
    bool result)
{
    //
}


void RallyStrategy::updateNextScene()
{
    //
}


void RallyStrategy::finish()
{
    mFinished = true;
}