#include "Scene.h"

EndingSceneState::EndingSceneState() : SceneState()
{
	Sound::getInstance()->loadSound("Resources\\Sounds\\endingscene.wav", "endingscene");
	Sound::getInstance()->play("endingscene", false, 1);
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
