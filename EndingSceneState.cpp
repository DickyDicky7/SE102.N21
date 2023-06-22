#include "Scene.h"

EndingSceneState::EndingSceneState() : SceneState()
{
	Sound::getInstance()->loadSound("Resources\\Sounds\\endingscene.wav", "endingscene");
}

EndingSceneState::~EndingSceneState()
{
}

void EndingSceneState::Exit(Scene& scene)
{
}

void EndingSceneState::Enter(Scene& scene)
{
	scene.stageIsReady = false;
	Sound::getInstance()->play("endingscene", false, 1);
}

void EndingSceneState::Render(Scene& scene)
{
	scene.SetAnimation(SCENE_ANIMATION_ID::ENDING_FRAME, scene.GetPosition(), scene.GetMovingDirection(), scene.GetAngle());
}

SceneState* EndingSceneState::Update(Scene& scene)
{
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
