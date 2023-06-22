#include "Scene.h"

CreditSceneState::CreditSceneState() : SceneState()
{
	Sound::getInstance()->loadSound("Resources\\Sounds\\endingscene.wav", "endingscene");
}

CreditSceneState::~CreditSceneState()
{
}

void CreditSceneState::Exit(Scene& scene)
{
}

void CreditSceneState::Enter(Scene& scene)
{
	scene.stageIsReady = false;
	Sound::getInstance()->play("endingscene", false, 1);
}

void CreditSceneState::Render(Scene& scene)
{
	if (++time >= 1000.0f)
	{
		T_CREDIT_5.Render();
		T_CREDIT_6.Render();
		T_CREDIT_7.Render();
		T_CREDIT_8.Render();
	}
	else
	if (++time >= 900.0f)
	{
		
	}
	else
	{
		T_CREDIT_1.Render();
		T_CREDIT_2.Render();
		T_CREDIT_3.Render();
		T_CREDIT_4.Render();
	}
}

SceneState* CreditSceneState::Update(Scene& scene)
{
	return NULL;
}

SceneState* CreditSceneState::HandleInput(Scene& scene, Input& input)
{
	return NULL;
}
