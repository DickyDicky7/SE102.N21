#include "Scene.h"

EndingSceneState::EndingSceneState() : SceneState()
{
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
