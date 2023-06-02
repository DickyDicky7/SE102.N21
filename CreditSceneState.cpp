#include "Scene.h"

CreditSceneState::CreditSceneState() : SceneState()
{
}

CreditSceneState::~CreditSceneState()
{
}

void CreditSceneState::Exit(Scene& scene)
{
}

void CreditSceneState::Enter(Scene& scene)
{
	scene.stageIsReady = false;
}

void CreditSceneState::Render(Scene& scene)
{
}

SceneState* CreditSceneState::Update(Scene& scene)
{
	return NULL;
}

SceneState* CreditSceneState::HandleInput(Scene& scene, Input& input)
{
	return NULL;
}
