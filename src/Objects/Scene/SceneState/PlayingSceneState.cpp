#include "Scene.h"

PlayingSceneState::PlayingSceneState() : SceneState()
{
}

PlayingSceneState::~PlayingSceneState()
{
}

void PlayingSceneState::Exit(Scene& scene)
{
	Sound::GetInstance()->Stop();
}

void PlayingSceneState::Enter(Scene& scene)
{
	scene.SetStageReady(true);
	scene.SetX(Constants::Scene::BLACK_OVERLAY_WIDTH * Constants::Scene::SCENE_CENTRE_WIDTH_FACTOR); scene.SetY(0.0f); scene.SetVX(+Constants::Scene::BLACK_TRANSITION_SPEED_X);
	Sound::GetInstance()->Stop();
	Sound::GetInstance()->Play("stage" + std::to_string(scene.GetCurrentStage()), true, 1);
}

void PlayingSceneState::Render(Scene& scene)
{
	scene.GetStage()->Render();
	for (int i = 0; i < *scene.GetLivesLeft(); i++)
	{
		GraphicsHelper::DrawSprite
		( GraphicsDatabase::sprites[SCENE_SPRITE_ID::LIFE]
		, D3DXVECTOR3( scene.GetStage()->GetCamera()->GetL() + AtCol(static_cast<float>(i) + Constants::Scene::UI_COLUMN_HUD_LIVES_ICONS_START)
		             , scene.GetStage()->GetCamera()->GetB() + AtRow(Constants::Scene::UI_ROW_HUD_LIVES_ICONS), 0.0f), DIRECTION::LEFT, 0.0f );
	}
	if (scene.GetX() <= Constants::Scene::BLACK_OVERLAY_WIDTH * Constants::Scene::SCENE_OFFSCREEN_WIDTH_FACTOR)
	{
		scene.SetAnimation(SCENE_ANIMATION_ID::BLACK, scene.GetPosition(), scene.GetMovingDirection(), scene.GetAngle());
	}
}

SceneState* PlayingSceneState::Update(Scene& scene)
{
	scene.GetStage()->Update();
	if (scene.GetStage()->HasDone())
	{
		if (++this->_turn == Constants::Scene::STAGE_CLEAR_TRANSITION_TURNS)
		{
			if (scene.GetCurrentStage() == Constants::Stages::STAGE_1_INDEX)
			{
				return new LoadingSceneState();
			}
			if (scene.GetCurrentStage() == Constants::Stages::STAGE_2_INDEX)
			{
				return new EndingSceneState();
			}
		}
	}
	if (*scene.GetLivesLeft() <= -1)
	{
		if (++this->_turn == Constants::Scene::GAME_OVER_TRANSITION_TURNS)
		{
			return new GameOverSceneState();
		}
	}
	if (scene.GetX() <= Constants::Scene::BLACK_OVERLAY_WIDTH * Constants::Scene::SCENE_OFFSCREEN_WIDTH_FACTOR)
	{
		auto result = Motion::CalculateUniformMotion({ scene.GetX(), scene.GetVX() });
		scene.SetX(result.coordinate);
	}
	return nullptr;
}

SceneState* PlayingSceneState::HandleInput(Scene& scene, Input& input)
{
	if (input.IsKey(DIK_ESCAPE))
	{
		return new LoadingSceneState();
	}
	scene.GetStage()->HandleInput(input);
	return nullptr;
}

