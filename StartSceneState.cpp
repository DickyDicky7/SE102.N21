#include "Scene.h"

StartSceneState::StartSceneState() : SceneState(), turn(0), chosen(false)
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
	if (chosen)
	{
		ULONGLONG now = GetTickCount64();
		if (now - time > 300)
		{
			GraphicsHelper::DrawSprite(GraphicsDatabase::sprites[SCENE_SPRITE_ID::BLANK], D3DXVECTOR3(83.0f, 80.0f, 0.0f), DIRECTION::LEFT, 0.0f);
			if (now - time > 600) time = now;
		}
	}
}

SceneState* StartSceneState::Update(Scene& scene)
{
	if (scene.GetX() > START_SCENE_W * 0.5f)
	{
		auto result = Motion::CalculateUniformMotion({ scene.GetX(), scene.GetVX() });
		scene.SetX(result.c);
	}
	if (chosen)
	{
		if (++turn == 300)
		{
			return new LoadingSceneState();
		}

	}
	return NULL;
}

SceneState* StartSceneState::HandleInput(Scene& scene, Input& input)
{
	if (scene.GetX() <= START_SCENE_W * 0.5f && (input.IsKey(DIK_RETURN) || input.IsKey(DIK_NUMPADENTER)))
	{
		chosen = true;
	}
	return NULL;
}

