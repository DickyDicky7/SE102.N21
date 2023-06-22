#include "Scene.h"

GameOverSceneState::GameOverSceneState() : SceneState(), T_CURRENT_SCORE(NULL), T_HIGHEST_SCORE(NULL), selection(SELECTION::CONTINUE), atRow(AtRow(10.5f))
{
}

GameOverSceneState::~GameOverSceneState()
{
	Destroy(T_CURRENT_SCORE);
	Destroy(T_HIGHEST_SCORE);
}

void GameOverSceneState::Exit(Scene& scene)
{
}

void GameOverSceneState::Enter(Scene& scene)
{
	scene.stageIsReady = false; *scene.livesLeft = 3;
	T_CURRENT_SCORE = new Text(std::to_string(scene.currentScore), AtRow(25.0f), AtCol(10.0f));
	T_HIGHEST_SCORE = new Text(std::to_string(scene.highestScore), AtRow(19.0f), AtCol(15.0f));
	Sound::getInstance()->play("gameOver", false, 1);
}

void GameOverSceneState::Render(Scene& scene)
{
	ULONGLONG now = GetTickCount64();
	if (now - time > 300)
	{
		T_CURRENT_SCORE->Render();
		T_HIGHEST_SCORE->Render();
		if (now - time > 600) time = now;
	}
	T_1P.Render(); T_HI.Render(); T_END.Render(); T_CONTINUE.Render(); T_GAME_OVER.Render();
	GraphicsHelper::DrawSprite(GraphicsDatabase::sprites[SCENE_SPRITE_ID::YELLOW_FALCON], D3DXVECTOR3(AtCol(11.0f), atRow, 0.0f), DIRECTION::LEFT, 0.0f);
}

SceneState* GameOverSceneState::Update(Scene& scene)
{
	return NULL;
}

SceneState* GameOverSceneState::HandleInput(Scene& scene, Input& input)
{
	if (input.IsKey(DIK_UP) && selection == SELECTION::END)
	{
		Sound::getInstance()->play("cursor", false, 1);
		selection = SELECTION::CONTINUE, atRow = AtRow(10.5f);
	}
	else
	if (input.IsKey(DIK_DOWN) && selection == SELECTION::CONTINUE)
	{
		Sound::getInstance()->play("cursor", false, 1);
		selection = SELECTION::END, atRow = AtRow(08.5f);
	}

	if (input.IsKey(DIK_RETURN))
	{
		switch (selection)
		{
		case SELECTION::CONTINUE: { 
			Sound::getInstance()->stop();
			Sound::getInstance()->play("select", false, 1);
			--scene.currentStage    ; scene.currentScore = 0; return new LoadingSceneState(); 
		}
		case SELECTION::END     : {   
			Sound::getInstance()->stop();
			Sound::getInstance()->play("select", false, 1);
			scene.currentStage = 0; scene.currentScore = 0; return new   StartSceneState(); 
		}
		}
	}

	return NULL;
}
