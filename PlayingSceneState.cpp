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
	scene.stageIsReady = true;
	scene.SetX(BLACK_W * 0.5f); scene.SetY(0.0f); scene.SetVX(+3.0f);
}

void PlayingSceneState::Render(Scene& scene)
{
	scene.stage->Render();
	for (int i = 0; i < *scene.livesLeft; i++)
	{
		GraphicsHelper::DrawSprite
		( GraphicsDatabase::sprites[SCENE_SPRITE_ID::LIFE]
		, D3DXVECTOR3( scene.stage->GetCamera()->GetL() + AtCol(i + 2)
		             , scene.stage->GetCamera()->GetB() + AtRow(27.0f), 0.0f), DIRECTION::LEFT, 0.0f );
	}
	if (scene.GetX() <= BLACK_W * 1.5f)
	{
		scene.SetAnimation(SCENE_ANIMATION_ID::BLACK, scene.GetPosition(), scene.GetMovingDirection(), scene.GetAngle());
	}
}

SceneState* PlayingSceneState::Update(Scene& scene)
{
	scene.stage->Update();
	if (*scene.livesLeft <= -1)
	{
		if (++turn == 300)
			return new GameOverSceneState();
	}
	if (scene.GetX() <= BLACK_W * 1.5f)
	{
		auto result = Motion::CalculateUniformMotion({ scene.GetX(), scene.GetVX() });
		scene.SetX(result.c);
	}
	return NULL;
}

SceneState* PlayingSceneState::HandleInput(Scene& scene, Input& input)
{
	if (input.IsKey(DIK_ESCAPE)) return new LoadingSceneState();
	scene.stage->HandleInput(input);
	return NULL;
}

