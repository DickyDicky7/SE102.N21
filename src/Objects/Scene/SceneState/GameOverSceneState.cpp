#include "Scene.h"

GameOverSceneState::GameOverSceneState() : SceneState(), _tCurrentScore(nullptr), _tHighestScore(nullptr), _selection(SELECTION::CONTINUE), _atRow(AtRow(Constants::Scene::GAME_OVER_SELECTION_ROW_CONTINUE))
{
}

GameOverSceneState::~GameOverSceneState()
{
	Destroy(this->_tCurrentScore);
	Destroy(this->_tHighestScore);
}

void GameOverSceneState::Exit(Scene& scene)
{
}

void GameOverSceneState::Enter(Scene& scene)
{
	scene.SetStageReady(false); *scene.GetLivesLeft() = Constants::Scene::CONTINUE_LIVES_COUNT;
	this->_tCurrentScore = new Text(std::to_string(scene.GetCurrentScore()), AtRow(Constants::Scene::UI_ROW_SCORE_VALUE), AtCol(Constants::Scene::UI_COLUMN_SCORE_VALUE));
	this->_tHighestScore = new Text(std::to_string(scene.GetHighestScore()), AtRow(Constants::Scene::UI_ROW_HIGH_SCORE_VALUE), AtCol(Constants::Scene::UI_COLUMN_HIGH_SCORE_VALUE));
	Sound::GetInstance()->Play("gameOver", false, 1);
}

void GameOverSceneState::Render(Scene& scene)
{
	ULONGLONG now = GetTickCount64();
	if (now - this->_time > Constants::Scene::TEXT_BLINK_INTERVAL_MILLISECONDS)
	{
		this->_tCurrentScore->Render();
		this->_tHighestScore->Render();
		if (now - this->_time > Constants::Scene::TEXT_BLINK_PERIOD_MILLISECONDS) this->_time = now;
	}
	this->_t1P.Render(); this->_tHi.Render(); this->_tEnd.Render(); this->_tContinue.Render(); this->_tGameOver.Render();
	GraphicsHelper::DrawSprite(GraphicsDatabase::sprites[SCENE_SPRITE_ID::YELLOW_FALCON], D3DXVECTOR3(AtCol(Constants::Scene::GAME_OVER_FALCON_CURSOR_COLUMN), this->_atRow, 0.0f), DIRECTION::LEFT, 0.0f);
}

SceneState* GameOverSceneState::Update(Scene& scene)
{
	return nullptr;
}

SceneState* GameOverSceneState::HandleInput(Scene& scene, Input& input)
{
	if (input.IsKey(DIK_UP) && this->_selection == SELECTION::END)
	{
		Sound::GetInstance()->Play("cursor", false, 1);
		this->_selection = SELECTION::CONTINUE, this->_atRow = AtRow(Constants::Scene::GAME_OVER_SELECTION_ROW_CONTINUE);
	}
	else
	if (input.IsKey(DIK_DOWN) && this->_selection == SELECTION::CONTINUE)
	{
		Sound::GetInstance()->Play("cursor", false, 1);
		this->_selection = SELECTION::END, this->_atRow = AtRow(Constants::Scene::GAME_OVER_SELECTION_ROW_END);
	}

	if (input.IsKey(DIK_RETURN))
	{
		switch (this->_selection)
		{
		case SELECTION::CONTINUE: {
			Sound::GetInstance()->Stop();
			Sound::GetInstance()->Play("select", false, 1);
			scene.SetCurrentStage(scene.GetCurrentStage() - 1); scene.SetCurrentScore(0); return new LoadingSceneState();
		}
		case SELECTION::END     : {
			Sound::GetInstance()->Stop();
			Sound::GetInstance()->Play("select", false, 1);
			scene.SetCurrentStage(0); scene.SetCurrentScore(0); return new   StartSceneState();
		}
		}
	}

	return nullptr;
}
