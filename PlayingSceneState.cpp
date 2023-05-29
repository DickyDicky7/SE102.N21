#include "Scene.h"

PlayingSceneState::PlayingSceneState() : SceneState()
{
}

PlayingSceneState::~PlayingSceneState()
{
}

void PlayingSceneState::Exit(Scene& scene)
{
}

void PlayingSceneState::Enter(Scene& scene)
{
}

void PlayingSceneState::Render(Scene& scene)
{
	scene.stage->Render();
}

SceneState* PlayingSceneState::Update(Scene& scene)
{
	scene.stage->Update();
	return NULL;
}

SceneState* PlayingSceneState::HandleInput(Scene& scene, Input& input)
{
	if (input.IsKey(DIK_ESCAPE)) return new LoadingSceneState();
	scene.stage->HandleInput(input);
	return NULL;
}

