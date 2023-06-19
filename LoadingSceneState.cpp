#include "Scene.h"
#include "Sound.h"
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


	Sound::getInstance()->loadSound("Resources\\Sounds\\clearStage.wav", "clearStage");
	Sound::getInstance()->play("clearStage", false, 1);

	Sound::getInstance()->loadSound("Resources\\Sounds\\stage1.wav", "stage1");
	Sound::getInstance()->loadSound("Resources\\Sounds\\stage2.wav", "stage2");
	Sound::getInstance()->loadSound("Resources\\Sounds\\stage3.wav", "stage3");
	Sound::getInstance()->loadSound("Resources\\Sounds\\beep.wav", "beep.wav");
	Sound::getInstance()->loadSound("Resources\\Sounds\\boss2bulletsound.wav", "boss2bulletsound.wav");
	Sound::getInstance()->loadSound("Resources\\Sounds\\boss2finaldestroy.wav", "boss2finaldestroy.wav");
	Sound::getInstance()->loadSound("Resources\\Sounds\\boss2finalhanddisappear.wav", "boss2finalhanddisappear.wav");
	Sound::getInstance()->loadSound("Resources\\Sounds\\bridgeexplosion.wav", "bridgeexplosion.wav");
	Sound::getInstance()->loadSound("Resources\\Sounds\\enemydead.wav", "enemydead.wav");
	Sound::getInstance()->loadSound("Resources\\Sounds\\warning.wav", "warning");
	Sound::getInstance()->loadSound("Resources\\Sounds\\explode.wav", "explode.wav");
	Sound::getInstance()->loadSound("Resources\\Sounds\\beShooted.wav", "beShooted");
	Sound::getInstance()->loadSound("Resources\\Sounds\\qexplode.wav", "qexplode");
	Sound::getInstance()->loadSound("Resources\\Sounds\\stonefailing.wav", "stonefailing");

	Sound::getInstance()->loadSound("Resources\\Sounds\\fallingmine.wav", "fallingmine");
	Sound::getInstance()->loadSound("Resources\\Sounds\\playerdie.wav", "playerdie");
	Sound::getInstance()->loadSound("Resources\\Sounds\\addlife.wav", "addlife");
	Sound::getInstance()->loadSound("Resources\\Sounds\\gameOver.wav", "gameOver");

	Sound::getInstance()->loadSound("Resources\\Sounds\\shootL.wav", "shootL");
	Sound::getInstance()->loadSound("Resources\\Sounds\\shootM.wav", "shootM");
	Sound::getInstance()->loadSound("Resources\\Sounds\\shootS.wav", "shootS");
	Sound::getInstance()->loadSound("Resources\\Sounds\\shootF.wav", "shootF");

	Sound::getInstance()->loadSound("Resources\\Sounds\\weaponL.wav", "weaponL");
	Sound::getInstance()->loadSound("Resources\\Sounds\\weaponM.wav", "weaponM");
	Sound::getInstance()->loadSound("Resources\\Sounds\\weaponS.wav", "weaponS");
	Sound::getInstance()->loadSound("Resources\\Sounds\\weaponF.wav", "weaponF");
	Sound::getInstance()->loadSound("Resources\\Sounds\\weaponB.wav", "weaponB");
	Sound::getInstance()->loadSound("Resources\\Sounds\\weaponR.wav", "weaponR");
	Sound::getInstance()->loadSound("Resources\\Sounds\\weaponD.wav", "weaponD");
	Sound::getInstance()->loadSound("Resources\\Sounds\\landing.wav", "landing");

	Sound::getInstance()->loadSound("Resources\\Sounds\\tank.wav", "tank");
	Sound::getInstance()->loadSound("Resources\\Sounds\\exbullet.wav", "exbullet");

	
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
