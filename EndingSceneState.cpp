#include "Scene.h"

EndingSceneState::EndingSceneState() : SceneState()
{
	Sound::getInstance()->loadSound("Resources\\Sounds\\rapid-2.wav", "rapid");
	Sound::getInstance()->loadSound("Resources\\Sounds\\bridgeexplosion.wav", "bridgeexplosion");
	delayToExplosion = 120;
}

EndingSceneState::~EndingSceneState()
{
	delayToExplosion = NULL;
}

void EndingSceneState::Exit(Scene& scene)
{
}

void EndingSceneState::Enter(Scene& scene)
{
	scene.stageIsReady = false;
	Sound::getInstance()->play("rapid", false, 1);
}

void EndingSceneState::Render(Scene& scene)
{
	scene.SetAnimation(SCENE_ANIMATION_ID::ENDING_FRAME, scene.GetPosition(), scene.GetMovingDirection(), scene.GetAngle());
}

SceneState* EndingSceneState::Update(Scene& scene)
{
	if (--delayToExplosion == 0)
		Sound::getInstance()->play("bridgeexplosion", false, 1);
	return NULL;
}

SceneState* EndingSceneState::HandleInput(Scene& scene, Input& input)
{
	return NULL;
}
