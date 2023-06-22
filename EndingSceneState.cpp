#include "Scene.h"

EndingSceneState::EndingSceneState() : SceneState()
{
	Sound::getInstance()->loadSound("Resources\\Sounds\\endingscene.wav", "endingscene");
}

EndingSceneState::~EndingSceneState()
{
}

void EndingSceneState::Exit(Scene& scene)
{
}

void EndingSceneState::Enter(Scene& scene)
{
	scene.stageIsReady = false;
	Sound::getInstance()->play("endingscene", false, 1);
}

void EndingSceneState::Render(Scene& scene)
{
}

SceneState* EndingSceneState::Update(Scene& scene)
{
	return NULL;
}

SceneState* EndingSceneState::HandleInput(Scene& scene, Input& input)
{
	return NULL;
}
