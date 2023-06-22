#include "Scene.h"

StartSceneState::StartSceneState() : SceneState(), chosen(false)
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
	scene.stageIsReady = false;
	scene.SetX(START_SCENE_W * 1.5f); scene.SetY(0.0f); scene.SetVX(-0.5f);

	Sound::getInstance()->loadSound("Resources\\Sounds\\cursor.wav", "cursor");
	Sound::getInstance()->loadSound("Resources\\Sounds\\select.wav", "select");
	Sound::getInstance()->loadSound("Resources\\Sounds\\exbullet.wav", "exbullet");
	Sound::getInstance()->loadSound("Resources\\Sounds\\startingscene.wav", "startingscene");
	Sound::getInstance()->play("startingscene", false, 1);
}

void StartSceneState::Render(Scene& scene)
{
	scene.SetAnimation(SCENE_ANIMATION_ID::START_SCENE, scene.GetPosition(), scene.GetMovingDirection(), scene.GetAngle());
	if (chosen)
	{
		ULONGLONG now = GetTickCount64();
		if (now - time > 300)
		{
			GraphicsHelper::DrawSprite(GraphicsDatabase::sprites[SCENE_SPRITE_ID::BLANK], D3DXVECTOR3(83.0f, 80.0f, 0.0f), DIRECTION::LEFT, 0.0f);
			if (now - time > 600) time = now;
		}
	}
	if (scene.GetX() <= START_SCENE_W * 0.5f)
	{
		GraphicsHelper::DrawSprite(GraphicsDatabase::sprites[SCENE_SPRITE_ID::YELLOW_FALCON], D3DXVECTOR3(25.0f, 79.0f, 0.0f), DIRECTION::LEFT, 0.0f);
	}
}

SceneState* StartSceneState::Update(Scene& scene)
{
	if (scene.GetX() > START_SCENE_W * 0.5f)
	{
		auto result = Motion::CalculateUniformMotion({ scene.GetX(), scene.GetVX() });
		scene.SetX(result.c);
	}
	if (chosen)
	{
		if (++turn == 300)
		{
			return new LoadingSceneState();
			//return new EndingSceneState();
		}

	}
	return NULL;
}

SceneState* StartSceneState::HandleInput(Scene& scene, Input& input)
{
	if (scene.GetX() <= START_SCENE_W * 0.5f && (input.IsKey(DIK_RETURN) || input.IsKey(DIK_NUMPADENTER)) && !chosen)
	{
		Sound::getInstance()->stop();
		Sound::getInstance()->play("exbullet", false, 1);
		chosen = true;
	}
	return NULL;
}

