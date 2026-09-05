#include "Scene.h"

StartSceneState::StartSceneState() : SceneState(), _chosen(false)
{
}

StartSceneState::~StartSceneState()
{
}

void StartSceneState::Exit(Scene& scene)
{
}

void StartSceneState::Enter(Scene& scene)
{
	scene.SetStageReady(false);
	scene.SetX(Constants::Scene::START_SCENE_WIDTH * Constants::Scene::SCENE_OFFSCREEN_WIDTH_FACTOR); scene.SetY(0.0f); scene.SetVX(Constants::Scene::START_SCENE_SPEED_X);

	Sound::GetInstance()->LoadSound("Resources\\Sounds\\cursor.wav", "cursor");
	Sound::GetInstance()->LoadSound("Resources\\Sounds\\select.wav", "select");
	Sound::GetInstance()->LoadSound("Resources\\Sounds\\exbullet.wav", "exbullet");
	Sound::GetInstance()->LoadSound("Resources\\Sounds\\startingscene.wav", "startingscene");
	Sound::GetInstance()->Play("startingscene", false, 1);
}

void StartSceneState::Render(Scene& scene)
{
	scene.SetAnimation(SCENE_ANIMATION_ID::START_SCENE, scene.GetPosition(), scene.GetMovingDirection(), scene.GetAngle());
	if (this->_chosen)
	{
		ULONGLONG now = GetTickCount64();
		if (now - this->_time > Constants::Scene::TEXT_BLINK_INTERVAL_MILLISECONDS)
		{
			GraphicsHelper::DrawSprite(GraphicsDatabase::sprites[SCENE_SPRITE_ID::BLANK], D3DXVECTOR3(Constants::Scene::START_SCENE_CURSOR_POSITION_X, Constants::Scene::START_SCENE_CURSOR_POSITION_Y, 0.0f), DIRECTION::LEFT, 0.0f);
			if (now - this->_time > Constants::Scene::TEXT_BLINK_PERIOD_MILLISECONDS) this->_time = now;
		}
	}
	if (scene.GetX() <= Constants::Scene::START_SCENE_WIDTH * Constants::Scene::SCENE_CENTRE_WIDTH_FACTOR)
	{
		GraphicsHelper::DrawSprite(GraphicsDatabase::sprites[SCENE_SPRITE_ID::YELLOW_FALCON], D3DXVECTOR3(Constants::Scene::START_SCENE_FALCON_LOGO_POSITION_X, Constants::Scene::START_SCENE_FALCON_LOGO_POSITION_Y, 0.0f), DIRECTION::LEFT, 0.0f);
	}
}

SceneState* StartSceneState::Update(Scene& scene)
{
	if (scene.GetX() > Constants::Scene::START_SCENE_WIDTH * Constants::Scene::SCENE_CENTRE_WIDTH_FACTOR)
	{
		auto result = Motion::CalculateUniformMotion({ scene.GetX(), scene.GetVX() });
		scene.SetX(result.coordinate);
	}
	if (this->_chosen)
	{
		if (++this->_turn == Constants::Scene::START_SCENE_TRANSITION_TURNS)
		{
			return new LoadingSceneState();
		}

	}
	return nullptr;
}

SceneState* StartSceneState::HandleInput(Scene& scene, Input& input)
{
	if (scene.GetX() <= Constants::Scene::START_SCENE_WIDTH * Constants::Scene::SCENE_CENTRE_WIDTH_FACTOR && (input.IsKey(DIK_RETURN) || input.IsKey(DIK_NUMPADENTER)) && !this->_chosen)
	{
		Sound::GetInstance()->Stop();
		Sound::GetInstance()->Play("exbullet", false, 1);
		this->_chosen = true;
	}
	return nullptr;
}

