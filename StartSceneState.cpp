#include "Scene.h"

StartSceneState::StartSceneState() : SceneState()
{
}

StartSceneState::~StartSceneState()
{
}

void StartSceneState::Exit(Scene& scene)
{
}

void StartSceneState::Enter(Scene& scene)
{
	scene.SetX(START_SCENE_W * 1.5f); scene.SetY(0.0f); scene.SetVX(-0.5f);
}

void StartSceneState::Render(Scene& scene)
{
	scene.SetAnimation(SCENE_ANIMATION_ID::START_SCENE, scene.GetPosition(), scene.GetMovingDirection(), scene.GetAngle());
}

SceneState* StartSceneState::Update(Scene& scene)
{
	if (scene.GetX() > START_SCENE_W * 0.5f)
	{
		auto result = Motion::CalculateUniformMotion({ scene.GetX(), scene.GetVX() });
		scene.SetX(result.c);
	}
	return NULL;
}

SceneState* StartSceneState::HandleInput(Scene& scene, Input& input)
{
	if (scene.GetX() <= START_SCENE_W * 0.5f && input.IsKey(DIK_SPACE))
		return new LoadingSceneState();
		return NULL;
}

