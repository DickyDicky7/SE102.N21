#include "Scene.h"

CreditSceneState::CreditSceneState() : SceneState()
{
	Sound::GetInstance()->LoadSound("Resources\\Sounds\\endingscene.wav", "endingscene");
}

CreditSceneState::~CreditSceneState()
{
}

void CreditSceneState::Exit(Scene& scene)
{
}

void CreditSceneState::Enter(Scene& scene)
{
	scene.SetStageReady(false);
	Sound::GetInstance()->Stop();
	Sound::GetInstance()->Play("endingscene", false, 1);
}

void CreditSceneState::Render(Scene& scene)
{
	++this->_time;

	if (this->_time >= Constants::Scene::CREDIT_PAGE_TWO_DURATION_TURNS)
	{
		this->_tCredit5.Render();
		this->_tCredit6.Render();
		this->_tCredit7.Render();
		this->_tCredit8.Render();
	}
	else
	if (this->_time >= Constants::Scene::CREDIT_PAGE_ONE_DURATION_TURNS)
	{

	}
	else
	{
		this->_tCredit1.Render();
		this->_tCredit2.Render();
		this->_tCredit3.Render();
		this->_tCredit4.Render();
	}
}

SceneState* CreditSceneState::Update(Scene& scene)
{
	return nullptr;
}

SceneState* CreditSceneState::HandleInput(Scene& scene, Input& input)
{
	return nullptr;
}
