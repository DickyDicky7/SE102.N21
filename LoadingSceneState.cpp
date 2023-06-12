#include "Scene.h"

LoadingSceneState::LoadingSceneState() : SceneState(), T_LIVES_LEFT(NULL), T_STAGE_NUMB(NULL), T_STAGE_NAME(NULL), T_CURRENT_SCORE(NULL), T_HIGHEST_SCORE(NULL)
{
}

LoadingSceneState::~LoadingSceneState()
{
	Destroy(T_LIVES_LEFT);
	Destroy(T_STAGE_NUMB);
	Destroy(T_STAGE_NAME);
	Destroy(T_CURRENT_SCORE);
	Destroy(T_HIGHEST_SCORE);
}

void LoadingSceneState::Exit(Scene& scene)
{
}

void LoadingSceneState::Enter(Scene& scene)
{
	scene.stageIsReady = false;

	if (++scene.currentStage > 2) 
		  scene.currentStage = 1;

	std::string stageNumb = "STAGE " 
		   + std::to_string(scene.currentStage);
	std::string stageName = scene.currentStage == 1 ? "JUNGLE" 
		                  : scene.currentStage == 2 ? "WATERFALL" : "";
	
	T_LIVES_LEFT    = new Text(std::to_string(*scene.livesLeft   ), AtRow(23.0f), AtCol(10.0f));
	T_STAGE_NUMB    = new Text(stageNumb                          , AtRow(14.0f), AtCol(12.0f));
	T_STAGE_NAME    = new Text(stageName                          , AtRow(12.0f), AtCol(12.0f));
	T_CURRENT_SCORE = new Text(std::to_string( scene.currentScore), AtRow(25.0f), AtCol(10.0f));
	T_HIGHEST_SCORE = new Text(std::to_string( scene.highestScore), AtRow(19.0f), AtCol(15.0f));

	if (scene.currentStage == 1)
	{
		Destroy(scene.stage);
		scene.stage = new Stage1();
		scene.stage->Load<TerrainStage1, CameraMovingForwardState>();
	}
	else
	if (scene.currentStage == 2)
	{
		Destroy(scene.stage);
		scene.stage = new Stage2();
		scene.stage->Load<TerrainStage2, CameraMovingUpwardState >();
	}

	if (scene.stage)
	{
		scene.stage->GetBill()->livesLeft = scene.livesLeft;
	}
}

void LoadingSceneState::Render(Scene& scene)
{
	T_1P.Render();
	T_HI.Render();
	T_REST.Render();
	T_STAGE_NUMB->Render();
	T_STAGE_NAME->Render();
	ULONGLONG now = GetTickCount64();
	if (now - time > 300)
	{
		T_LIVES_LEFT->Render();
		T_CURRENT_SCORE->Render();
		T_HIGHEST_SCORE->Render();
		if (now - time > 600) time = now;
	}
}

SceneState* LoadingSceneState::Update(Scene& scene)
{
	if (++turn == 300) return new PlayingSceneState();
	return NULL;
}

SceneState* LoadingSceneState::HandleInput(Scene& scene, Input& input)
{
	return NULL;
}
