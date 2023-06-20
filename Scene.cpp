#include "Scene.h"

Scene::Scene() : Entity(), HasTextures(), HasSprites(), HasAnimations(), state(NULL), updateState(NULL), handleInputState(NULL), stage(NULL), livesLeft(new INT(50)), currentStage(0), currentScore(0), highestScore(20000), stageIsReady(false)
{
	ChangeState(state, new StartSceneState(), this);
	this->LoadTextures(); this->LoadSprites(); this->LoadAnimations();
	Letter representativeLetter(""); representativeLetter.LoadTextures(); representativeLetter.LoadSprites(); representativeLetter.LoadAnimations();
}

Scene::~Scene()
{
	Destroy(state);
	Destroy(updateState);
	Destroy(handleInputState);
}

void Scene::Update()
{
	updateState = state->Update(*this);
}

void Scene::Render()
{
	state->Render(*this);
	this->w = this->currentFrameW;
	this->h = this->currentFrameH;

	if (updateState)
	{
		ChangeState(state, updateState, this);
		updateState = NULL;
	}
	if (handleInputState)
	{
		ChangeState(state, handleInputState, this);
		handleInputState = NULL;
	}
}

void Scene::HandleInput(Input& input)
{
	handleInputState = state->HandleInput(*this, input);
}

void Scene::LoadSprites()
{
	if (HasSprites<Scene>::hasBeenLoaded.value) return;
	HasSprites<Scene>::hasBeenLoaded.value = true;

	GraphicsHelper::InsertSprite(SCENE_SPRITE_ID::LIFE         , 000, 015, 030, 020, DIRECTION::LEFT, SCENE_TEXTURE_ID::LIFE         );
	GraphicsHelper::InsertSprite(SCENE_SPRITE_ID::BLACK        , 000, 000, 256, 240, DIRECTION::LEFT, SCENE_TEXTURE_ID::BLACK        );
	GraphicsHelper::InsertSprite(SCENE_SPRITE_ID::BLANK        , 000, 000, 100, 013, DIRECTION::LEFT, SCENE_TEXTURE_ID::BLANK        );
	GraphicsHelper::InsertSprite(SCENE_SPRITE_ID::START_SCENE  , 000, 000, 256, 224, DIRECTION::LEFT, SCENE_TEXTURE_ID::START_SCENE  );
	GraphicsHelper::InsertSprite(SCENE_SPRITE_ID::YELLOW_FALCON, 000, 000,  18, 014, DIRECTION::LEFT, SCENE_TEXTURE_ID::YELLOW_FALCON);

	OutputDebugString(L"Scene Sprites Loaded Successfully\n");
}

void Scene::LoadTextures()
{
	if (HasTextures<Scene>::hasBeenLoaded.value) return;
	HasTextures<Scene>::hasBeenLoaded.value = true;

	GraphicsHelper::InsertTexure(SCENE_TEXTURE_ID::LIFE         , L"Resources\\Textures\\Life.bmp"        );
	GraphicsHelper::InsertTexure(SCENE_TEXTURE_ID::BLACK        , L"Resources\\Textures\\Black.bmp"       );
	GraphicsHelper::InsertTexure(SCENE_TEXTURE_ID::BLANK        , L"Resources\\Textures\\Blank.bmp"       );
	GraphicsHelper::InsertTexure(SCENE_TEXTURE_ID::START_SCENE  , L"Resources\\Textures\\StartScene.bmp"  );
	GraphicsHelper::InsertTexure(SCENE_TEXTURE_ID::YELLOW_FALCON, L"Resources\\Textures\\YellowFalcon.bmp");

	OutputDebugString(L"Scene Textures Loaded Successfully\n");
}

void Scene::LoadAnimations()
{
	if (HasAnimations<Scene>::hasBeenLoaded.value) return;
	HasAnimations<Scene>::hasBeenLoaded.value = true;

	GraphicsHelper::InsertAnimation(SCENE_ANIMATION_ID::LIFE         , 300, { { SCENE_SPRITE_ID::LIFE         , 0 } });
	GraphicsHelper::InsertAnimation(SCENE_ANIMATION_ID::BLACK        , 300, { { SCENE_SPRITE_ID::BLACK        , 0 } });
	GraphicsHelper::InsertAnimation(SCENE_ANIMATION_ID::BLANK        , 300, { { SCENE_SPRITE_ID::BLANK        , 0 } });
	GraphicsHelper::InsertAnimation(SCENE_ANIMATION_ID::START_SCENE  , 300, { { SCENE_SPRITE_ID::START_SCENE  , 0 } });
	GraphicsHelper::InsertAnimation(SCENE_ANIMATION_ID::YELLOW_FALCON, 300, { { SCENE_SPRITE_ID::YELLOW_FALCON, 0 } });

	OutputDebugString(L"Scene Animations Loaded Successfully\n");
}
