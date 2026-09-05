#include "Scene.h"
#include "Sound.h"
LoadingSceneState::LoadingSceneState() : SceneState(), _tLivesLeft(nullptr), _tStageNumb(nullptr), _tStageName(nullptr), _tCurrentScore(nullptr), _tHighestScore(nullptr)
{
}

LoadingSceneState::~LoadingSceneState()
{
	Destroy(this->_tLivesLeft);
	Destroy(this->_tStageNumb);
	Destroy(this->_tStageName);
	Destroy(this->_tCurrentScore);
	Destroy(this->_tHighestScore);
}

void LoadingSceneState::Exit(Scene& scene)
{
}

void LoadingSceneState::Enter(Scene& scene)
{
	scene.SetStageReady(false);

	scene.SetCurrentStage(scene.GetCurrentStage() + 1);
	if (scene.GetCurrentStage() > Constants::Stages::TOTAL_STAGES)
		scene.SetCurrentStage(Constants::Stages::STAGE_1_INDEX);

	const int currentStage = scene.GetCurrentStage();

	std::string stageNumb = "STAGE "
		   + std::to_string(currentStage);
	std::string stageName = currentStage == Constants::Stages::STAGE_1_INDEX ? "JUNGLE"
		                  : currentStage == Constants::Stages::STAGE_2_INDEX ? "WATERFALL" : "";

	this->_tLivesLeft    = new Text(std::to_string(*scene.GetLivesLeft()   ), AtRow(Constants::Scene::UI_ROW_LIVES_LEFT_VALUE), AtCol(Constants::Scene::UI_COLUMN_LIVES_LEFT_VALUE));
	this->_tStageNumb    = new Text(stageNumb                          , AtRow(Constants::Scene::UI_ROW_STAGE_NUMBER), AtCol(Constants::Scene::UI_COLUMN_STAGE_NUMBER));
	this->_tStageName    = new Text(stageName                          , AtRow(Constants::Scene::UI_ROW_STAGE_NAME), AtCol(Constants::Scene::UI_COLUMN_STAGE_NAME));
	this->_tCurrentScore = new Text(std::to_string( scene.GetCurrentScore()), AtRow(Constants::Scene::UI_ROW_SCORE_VALUE), AtCol(Constants::Scene::UI_COLUMN_SCORE_VALUE));
	this->_tHighestScore = new Text(std::to_string( scene.GetHighestScore()), AtRow(Constants::Scene::UI_ROW_HIGH_SCORE_VALUE), AtCol(Constants::Scene::UI_COLUMN_HIGH_SCORE_VALUE));

	if (currentStage == Constants::Stages::STAGE_1_INDEX)
	{
		scene.ReplaceStage(new Stage1());
		scene.GetStage()->Load<TerrainStage1, CameraMovingForwardState>();
	}
	else
	if (currentStage == Constants::Stages::STAGE_2_INDEX)
	{
		scene.ReplaceStage(new Stage2());
		scene.GetStage()->Load<TerrainStage2, CameraMovingUpwardState >();
	}

	if (scene.GetStage())
	{
		scene.GetStage()->GetBill()->SetLivesLeft(scene.GetLivesLeft());
	}


	static const struct SoundResource {
		const char* path;
		const char* id;
	} soundResources[] = {
		{ "Resources\\Sounds\\clearStage.wav", "clearStage" },
		{ "Resources\\Sounds\\stage1.wav", "stage1" },
		{ "Resources\\Sounds\\stage2.wav", "stage2" },
		{ "Resources\\Sounds\\stage3.wav", "stage3" },
		{ "Resources\\Sounds\\beep.wav", "beep" },
		{ "Resources\\Sounds\\boss1dead.wav", "boss1dead" },
		{ "Resources\\Sounds\\boss2bulletsound.wav", "boss2bulletsound" },
		{ "Resources\\Sounds\\boss2finaldestroy.wav", "boss2finaldestroy" },
		{ "Resources\\Sounds\\boss2finalhanddisappear.wav", "boss2finalhanddisappear" },
		{ "Resources\\Sounds\\bridgeexplosion.wav", "bridgeexplosion" },
		{ "Resources\\Sounds\\enemydead.wav", "enemydead" },
		{ "Resources\\Sounds\\warning.wav", "warning" },
		{ "Resources\\Sounds\\explode.wav", "explode" },
		{ "Resources\\Sounds\\beShooted.wav", "beShooted" },
		{ "Resources\\Sounds\\qexplode.wav", "qexplode" },
		{ "Resources\\Sounds\\stonefailing.wav", "stonefailing" },
		{ "Resources\\Sounds\\fallingmine.wav", "fallingmine" },
		{ "Resources\\Sounds\\playerdie.wav", "playerdie" },
		{ "Resources\\Sounds\\addlife.wav", "addlife" },
		{ "Resources\\Sounds\\gameOver.wav", "gameOver" },
		{ "Resources\\Sounds\\passboss.wav", "passboss" },
		{ "Resources\\Sounds\\shootL.wav", "shootL" },
		{ "Resources\\Sounds\\shootM.wav", "shootM" },
		{ "Resources\\Sounds\\shootS.wav", "shootS" },
		{ "Resources\\Sounds\\shootF.wav", "shootF" },
		{ "Resources\\Sounds\\weaponL.wav", "weaponL" },
		{ "Resources\\Sounds\\weaponM.wav", "weaponM" },
		{ "Resources\\Sounds\\weaponS.wav", "weaponS" },
		{ "Resources\\Sounds\\weaponF.wav", "weaponF" },
		{ "Resources\\Sounds\\weaponB.wav", "weaponB" },
		{ "Resources\\Sounds\\weaponR.wav", "weaponR" },
		{ "Resources\\Sounds\\weaponD.wav", "weaponD" },
		{ "Resources\\Sounds\\landing.wav", "landing" },
		{ "Resources\\Sounds\\tank.wav", "tank" },
		{ "Resources\\Sounds\\exbullet.wav", "exbullet" },
	};

	for (const auto& res : soundResources)
	{
		Sound::GetInstance()->LoadSound(res.path, res.id);
	}

	Sound::GetInstance()->Play("clearStage", false, 1);
}

void LoadingSceneState::Render(Scene& scene)
{
	this->_t1P.Render();
	this->_tHi.Render();
	this->_tRest.Render();
	this->_tStageNumb->Render();
	this->_tStageName->Render();
	ULONGLONG now = GetTickCount64();
	if (now - this->_time > Constants::Scene::TEXT_BLINK_INTERVAL_MILLISECONDS)
	{
		this->_tLivesLeft->Render();
		this->_tCurrentScore->Render();
		this->_tHighestScore->Render();
		if (now - this->_time > Constants::Scene::TEXT_BLINK_PERIOD_MILLISECONDS) this->_time = now;
	}
}

SceneState* LoadingSceneState::Update(Scene& scene)
{
	if (++this->_turn == Constants::Scene::LOADING_SCENE_TRANSITION_TURNS) return new PlayingSceneState();
	return nullptr;
}

SceneState* LoadingSceneState::HandleInput(Scene& scene, Input& input)
{
	return nullptr;
}
