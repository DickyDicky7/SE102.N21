#include "Scene.h"

EndingSceneState::EndingSceneState() : SceneState()
{
	Sound::getInstance()->loadSound("Resources\\Sounds\\aircraft.wav", "aircraft");
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
	scene.SetX(+IN_GAME_SCREEN_W * 0.5f);
	scene.SetY(-IN_GAME_SCREEN_H * 0.5f);
	Sound::getInstance()->play("aircraft", false, 1);
}

void EndingSceneState::Render(Scene& scene)
{
	scene.SetAnimation(SCENE_ANIMATION_ID::ENDING_FRAME, scene.GetPosition(), scene.GetMovingDirection(), scene.GetAngle());
}

SceneState* EndingSceneState::Update(Scene& scene)
{
	if (--delayToExplosion == 0)
		Sound::getInstance()->play("bridgeexplosion", false, 1);

	std::vector<std::tuple<SPRITE_ID, TIME>>& frames = std::get<
		std::vector<std::tuple<SPRITE_ID, TIME>>>(GraphicsDatabase::animations[SCENE_ANIMATION_ID::ENDING_FRAME]);

	if (std::cmp_greater_equal(scene.GetCurrentFrame() + 1, frames.size()))
	{
		return new CreditSceneState();
	}
	return NULL;
}

SceneState* EndingSceneState::HandleInput(Scene& scene, Input& input)
{
	return NULL;
}
