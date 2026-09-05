#include "Scene.h"

EndingSceneState::EndingSceneState() : SceneState()
{
	Sound::GetInstance()->LoadSound("Resources\\Sounds\\aircraft.wav", "aircraft");
	Sound::GetInstance()->LoadSound("Resources\\Sounds\\bridgeexplosion.wav", "bridgeexplosion");
	this->_delayToExplosion = Constants::Scene::ENDING_EXPLOSION_DELAY_FRAMES;
}

EndingSceneState::~EndingSceneState()
{
	this->_delayToExplosion = 0;
}

void EndingSceneState::Exit(Scene& scene)
{
}

void EndingSceneState::Enter(Scene& scene)
{
	scene.SetStageReady(false);
	scene.SetX(Constants::Screen::IN_GAME_WIDTH * Constants::Scene::SCENE_CENTRE_WIDTH_FACTOR);
	scene.SetY(0.0f);
	Sound::GetInstance()->Play("aircraft", false, 1);
}

void EndingSceneState::Render(Scene& scene)
{
	scene.SetAnimation(SCENE_ANIMATION_ID::ENDING_FRAME, scene.GetPosition(), scene.GetMovingDirection(), scene.GetAngle());
}

SceneState* EndingSceneState::Update(Scene& scene)
{
	if (--this->_delayToExplosion == 0)
		Sound::GetInstance()->Play("bridgeexplosion", false, 1);

	std::vector<std::tuple<SPRITE_ID, TIME>>& frames = std::get<
		std::vector<std::tuple<SPRITE_ID, TIME>>>(GraphicsDatabase::animations[SCENE_ANIMATION_ID::ENDING_FRAME]);

	if (std::cmp_greater_equal(scene.GetCurrentFrame() + 1, frames.size()))
	{
		return new CreditSceneState();
	}
	return nullptr;
}

SceneState* EndingSceneState::HandleInput(Scene& scene, Input& input)
{
	return nullptr;
}
