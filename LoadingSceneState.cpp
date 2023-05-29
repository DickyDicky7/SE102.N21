#include "Scene.h"

LoadingSceneState::LoadingSceneState() : SceneState()
{
}

LoadingSceneState::~LoadingSceneState()
{
}

void LoadingSceneState::Exit(Scene& scene)
{
}

void LoadingSceneState::Enter(Scene& scene)
{
}

void LoadingSceneState::Render(Scene& scene)
{
	T_1P.Render(); T_HI.Render(); T_REST.Render();
}

SceneState* LoadingSceneState::Update(Scene& scene)
{
	return NULL;
}

SceneState* LoadingSceneState::HandleInput(Scene& scene, Input& input)
{
	return NULL;
}
