#include "Scene.h"

Scene::Scene() : Entity(), HasTextures(), HasSprites(), HasAnimations(), state(NULL), updateState(NULL), handleInputState(NULL), stage(NULL), livesLeft(new INT(50)), currentStage(0), currentScore(0), highestScore(20000), stageIsReady(false)
{
	ChangeState(state, new EndingSceneState(), this);
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

	GraphicsHelper::InsertSprite(SCENE_SPRITE_ID::ENDING_FRAME_00, 000, 000, 640, 360, DIRECTION::LEFT, SCENE_TEXTURE_ID::ENDING_FRAME_00);
	GraphicsHelper::InsertSprite(SCENE_SPRITE_ID::ENDING_FRAME_00, 000, 000, 640, 360, DIRECTION::LEFT, SCENE_TEXTURE_ID::ENDING_FRAME_00);
	GraphicsHelper::InsertSprite(SCENE_SPRITE_ID::ENDING_FRAME_01, 000, 000, 640, 360, DIRECTION::LEFT, SCENE_TEXTURE_ID::ENDING_FRAME_01);
	GraphicsHelper::InsertSprite(SCENE_SPRITE_ID::ENDING_FRAME_02, 000, 000, 640, 360, DIRECTION::LEFT, SCENE_TEXTURE_ID::ENDING_FRAME_02);
	GraphicsHelper::InsertSprite(SCENE_SPRITE_ID::ENDING_FRAME_03, 000, 000, 640, 360, DIRECTION::LEFT, SCENE_TEXTURE_ID::ENDING_FRAME_03);
	GraphicsHelper::InsertSprite(SCENE_SPRITE_ID::ENDING_FRAME_04, 000, 000, 640, 360, DIRECTION::LEFT, SCENE_TEXTURE_ID::ENDING_FRAME_04);
	GraphicsHelper::InsertSprite(SCENE_SPRITE_ID::ENDING_FRAME_05, 000, 000, 640, 360, DIRECTION::LEFT, SCENE_TEXTURE_ID::ENDING_FRAME_05);
	GraphicsHelper::InsertSprite(SCENE_SPRITE_ID::ENDING_FRAME_06, 000, 000, 640, 360, DIRECTION::LEFT, SCENE_TEXTURE_ID::ENDING_FRAME_06);
	GraphicsHelper::InsertSprite(SCENE_SPRITE_ID::ENDING_FRAME_07, 000, 000, 640, 360, DIRECTION::LEFT, SCENE_TEXTURE_ID::ENDING_FRAME_07);
	GraphicsHelper::InsertSprite(SCENE_SPRITE_ID::ENDING_FRAME_08, 000, 000, 640, 360, DIRECTION::LEFT, SCENE_TEXTURE_ID::ENDING_FRAME_08);
	GraphicsHelper::InsertSprite(SCENE_SPRITE_ID::ENDING_FRAME_09, 000, 000, 640, 360, DIRECTION::LEFT, SCENE_TEXTURE_ID::ENDING_FRAME_09);
	GraphicsHelper::InsertSprite(SCENE_SPRITE_ID::ENDING_FRAME_10, 000, 000, 640, 360, DIRECTION::LEFT, SCENE_TEXTURE_ID::ENDING_FRAME_10);
	GraphicsHelper::InsertSprite(SCENE_SPRITE_ID::ENDING_FRAME_11, 000, 000, 640, 360, DIRECTION::LEFT, SCENE_TEXTURE_ID::ENDING_FRAME_11);
	GraphicsHelper::InsertSprite(SCENE_SPRITE_ID::ENDING_FRAME_12, 000, 000, 640, 360, DIRECTION::LEFT, SCENE_TEXTURE_ID::ENDING_FRAME_12);
	GraphicsHelper::InsertSprite(SCENE_SPRITE_ID::ENDING_FRAME_13, 000, 000, 640, 360, DIRECTION::LEFT, SCENE_TEXTURE_ID::ENDING_FRAME_13);
	GraphicsHelper::InsertSprite(SCENE_SPRITE_ID::ENDING_FRAME_14, 000, 000, 640, 360, DIRECTION::LEFT, SCENE_TEXTURE_ID::ENDING_FRAME_14);
	GraphicsHelper::InsertSprite(SCENE_SPRITE_ID::ENDING_FRAME_15, 000, 000, 640, 360, DIRECTION::LEFT, SCENE_TEXTURE_ID::ENDING_FRAME_15);
	GraphicsHelper::InsertSprite(SCENE_SPRITE_ID::ENDING_FRAME_16, 000, 000, 640, 360, DIRECTION::LEFT, SCENE_TEXTURE_ID::ENDING_FRAME_16);
	GraphicsHelper::InsertSprite(SCENE_SPRITE_ID::ENDING_FRAME_17, 000, 000, 640, 360, DIRECTION::LEFT, SCENE_TEXTURE_ID::ENDING_FRAME_17);
	GraphicsHelper::InsertSprite(SCENE_SPRITE_ID::ENDING_FRAME_18, 000, 000, 640, 360, DIRECTION::LEFT, SCENE_TEXTURE_ID::ENDING_FRAME_18);
	GraphicsHelper::InsertSprite(SCENE_SPRITE_ID::ENDING_FRAME_19, 000, 000, 640, 360, DIRECTION::LEFT, SCENE_TEXTURE_ID::ENDING_FRAME_19);
	GraphicsHelper::InsertSprite(SCENE_SPRITE_ID::ENDING_FRAME_20, 000, 000, 640, 360, DIRECTION::LEFT, SCENE_TEXTURE_ID::ENDING_FRAME_20);
	GraphicsHelper::InsertSprite(SCENE_SPRITE_ID::ENDING_FRAME_21, 000, 000, 640, 360, DIRECTION::LEFT, SCENE_TEXTURE_ID::ENDING_FRAME_21);
	GraphicsHelper::InsertSprite(SCENE_SPRITE_ID::ENDING_FRAME_22, 000, 000, 640, 360, DIRECTION::LEFT, SCENE_TEXTURE_ID::ENDING_FRAME_22);
	GraphicsHelper::InsertSprite(SCENE_SPRITE_ID::ENDING_FRAME_23, 000, 000, 640, 360, DIRECTION::LEFT, SCENE_TEXTURE_ID::ENDING_FRAME_23);
	GraphicsHelper::InsertSprite(SCENE_SPRITE_ID::ENDING_FRAME_24, 000, 000, 640, 360, DIRECTION::LEFT, SCENE_TEXTURE_ID::ENDING_FRAME_24);
	GraphicsHelper::InsertSprite(SCENE_SPRITE_ID::ENDING_FRAME_25, 000, 000, 640, 360, DIRECTION::LEFT, SCENE_TEXTURE_ID::ENDING_FRAME_25);
	GraphicsHelper::InsertSprite(SCENE_SPRITE_ID::ENDING_FRAME_26, 000, 000, 640, 360, DIRECTION::LEFT, SCENE_TEXTURE_ID::ENDING_FRAME_26);
	GraphicsHelper::InsertSprite(SCENE_SPRITE_ID::ENDING_FRAME_27, 000, 000, 640, 360, DIRECTION::LEFT, SCENE_TEXTURE_ID::ENDING_FRAME_27);
	GraphicsHelper::InsertSprite(SCENE_SPRITE_ID::ENDING_FRAME_28, 000, 000, 640, 360, DIRECTION::LEFT, SCENE_TEXTURE_ID::ENDING_FRAME_28);
	GraphicsHelper::InsertSprite(SCENE_SPRITE_ID::ENDING_FRAME_29, 000, 000, 640, 360, DIRECTION::LEFT, SCENE_TEXTURE_ID::ENDING_FRAME_29);
	GraphicsHelper::InsertSprite(SCENE_SPRITE_ID::ENDING_FRAME_30, 000, 000, 640, 360, DIRECTION::LEFT, SCENE_TEXTURE_ID::ENDING_FRAME_30);
	GraphicsHelper::InsertSprite(SCENE_SPRITE_ID::ENDING_FRAME_31, 000, 000, 640, 360, DIRECTION::LEFT, SCENE_TEXTURE_ID::ENDING_FRAME_31);
	GraphicsHelper::InsertSprite(SCENE_SPRITE_ID::ENDING_FRAME_32, 000, 000, 640, 360, DIRECTION::LEFT, SCENE_TEXTURE_ID::ENDING_FRAME_32);
	GraphicsHelper::InsertSprite(SCENE_SPRITE_ID::ENDING_FRAME_33, 000, 000, 640, 360, DIRECTION::LEFT, SCENE_TEXTURE_ID::ENDING_FRAME_33);
	GraphicsHelper::InsertSprite(SCENE_SPRITE_ID::ENDING_FRAME_34, 000, 000, 640, 360, DIRECTION::LEFT, SCENE_TEXTURE_ID::ENDING_FRAME_34);
	GraphicsHelper::InsertSprite(SCENE_SPRITE_ID::ENDING_FRAME_35, 000, 000, 640, 360, DIRECTION::LEFT, SCENE_TEXTURE_ID::ENDING_FRAME_35);
	GraphicsHelper::InsertSprite(SCENE_SPRITE_ID::ENDING_FRAME_36, 000, 000, 640, 360, DIRECTION::LEFT, SCENE_TEXTURE_ID::ENDING_FRAME_36);
	GraphicsHelper::InsertSprite(SCENE_SPRITE_ID::ENDING_FRAME_37, 000, 000, 640, 360, DIRECTION::LEFT, SCENE_TEXTURE_ID::ENDING_FRAME_37);
	GraphicsHelper::InsertSprite(SCENE_SPRITE_ID::ENDING_FRAME_38, 000, 000, 640, 360, DIRECTION::LEFT, SCENE_TEXTURE_ID::ENDING_FRAME_38);
	GraphicsHelper::InsertSprite(SCENE_SPRITE_ID::ENDING_FRAME_39, 000, 000, 640, 360, DIRECTION::LEFT, SCENE_TEXTURE_ID::ENDING_FRAME_39);
	GraphicsHelper::InsertSprite(SCENE_SPRITE_ID::ENDING_FRAME_40, 000, 000, 640, 360, DIRECTION::LEFT, SCENE_TEXTURE_ID::ENDING_FRAME_40);
	GraphicsHelper::InsertSprite(SCENE_SPRITE_ID::ENDING_FRAME_41, 000, 000, 640, 360, DIRECTION::LEFT, SCENE_TEXTURE_ID::ENDING_FRAME_41);
	GraphicsHelper::InsertSprite(SCENE_SPRITE_ID::ENDING_FRAME_42, 000, 000, 640, 360, DIRECTION::LEFT, SCENE_TEXTURE_ID::ENDING_FRAME_42);
	GraphicsHelper::InsertSprite(SCENE_SPRITE_ID::ENDING_FRAME_43, 000, 000, 640, 360, DIRECTION::LEFT, SCENE_TEXTURE_ID::ENDING_FRAME_43);
	GraphicsHelper::InsertSprite(SCENE_SPRITE_ID::ENDING_FRAME_44, 000, 000, 640, 360, DIRECTION::LEFT, SCENE_TEXTURE_ID::ENDING_FRAME_44);
	GraphicsHelper::InsertSprite(SCENE_SPRITE_ID::ENDING_FRAME_45, 000, 000, 640, 360, DIRECTION::LEFT, SCENE_TEXTURE_ID::ENDING_FRAME_45);
	GraphicsHelper::InsertSprite(SCENE_SPRITE_ID::ENDING_FRAME_46, 000, 000, 640, 360, DIRECTION::LEFT, SCENE_TEXTURE_ID::ENDING_FRAME_46);
	GraphicsHelper::InsertSprite(SCENE_SPRITE_ID::ENDING_FRAME_47, 000, 000, 640, 360, DIRECTION::LEFT, SCENE_TEXTURE_ID::ENDING_FRAME_47);
	GraphicsHelper::InsertSprite(SCENE_SPRITE_ID::ENDING_FRAME_48, 000, 000, 640, 360, DIRECTION::LEFT, SCENE_TEXTURE_ID::ENDING_FRAME_48);
	GraphicsHelper::InsertSprite(SCENE_SPRITE_ID::ENDING_FRAME_49, 000, 000, 640, 360, DIRECTION::LEFT, SCENE_TEXTURE_ID::ENDING_FRAME_49);
	GraphicsHelper::InsertSprite(SCENE_SPRITE_ID::ENDING_FRAME_50, 000, 000, 640, 360, DIRECTION::LEFT, SCENE_TEXTURE_ID::ENDING_FRAME_50);
	GraphicsHelper::InsertSprite(SCENE_SPRITE_ID::ENDING_FRAME_51, 000, 000, 640, 360, DIRECTION::LEFT, SCENE_TEXTURE_ID::ENDING_FRAME_51);

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

	GraphicsHelper::InsertTexure(SCENE_TEXTURE_ID::ENDING_FRAME_00, L"Resources\\Textures\\ending_frame\\frame_00_delay-0.1s.bmp");
	GraphicsHelper::InsertTexure(SCENE_TEXTURE_ID::ENDING_FRAME_01, L"Resources\\Textures\\ending_frame\\frame_01_delay-0.1s.bmp");
	GraphicsHelper::InsertTexure(SCENE_TEXTURE_ID::ENDING_FRAME_02, L"Resources\\Textures\\ending_frame\\frame_02_delay-0.1s.bmp");
	GraphicsHelper::InsertTexure(SCENE_TEXTURE_ID::ENDING_FRAME_03, L"Resources\\Textures\\ending_frame\\frame_03_delay-0.1s.bmp");
	GraphicsHelper::InsertTexure(SCENE_TEXTURE_ID::ENDING_FRAME_04, L"Resources\\Textures\\ending_frame\\frame_04_delay-0.1s.bmp");
	GraphicsHelper::InsertTexure(SCENE_TEXTURE_ID::ENDING_FRAME_05, L"Resources\\Textures\\ending_frame\\frame_05_delay-0.1s.bmp");
	GraphicsHelper::InsertTexure(SCENE_TEXTURE_ID::ENDING_FRAME_06, L"Resources\\Textures\\ending_frame\\frame_06_delay-0.1s.bmp");
	GraphicsHelper::InsertTexure(SCENE_TEXTURE_ID::ENDING_FRAME_07, L"Resources\\Textures\\ending_frame\\frame_07_delay-0.1s.bmp");
	GraphicsHelper::InsertTexure(SCENE_TEXTURE_ID::ENDING_FRAME_08, L"Resources\\Textures\\ending_frame\\frame_08_delay-0.1s.bmp");
	GraphicsHelper::InsertTexure(SCENE_TEXTURE_ID::ENDING_FRAME_09, L"Resources\\Textures\\ending_frame\\frame_09_delay-0.1s.bmp");
	GraphicsHelper::InsertTexure(SCENE_TEXTURE_ID::ENDING_FRAME_10, L"Resources\\Textures\\ending_frame\\frame_10_delay-0.1s.bmp");
	GraphicsHelper::InsertTexure(SCENE_TEXTURE_ID::ENDING_FRAME_11, L"Resources\\Textures\\ending_frame\\frame_11_delay-0.1s.bmp");
	GraphicsHelper::InsertTexure(SCENE_TEXTURE_ID::ENDING_FRAME_12, L"Resources\\Textures\\ending_frame\\frame_12_delay-0.1s.bmp");
	GraphicsHelper::InsertTexure(SCENE_TEXTURE_ID::ENDING_FRAME_13, L"Resources\\Textures\\ending_frame\\frame_13_delay-0.1s.bmp");
	GraphicsHelper::InsertTexure(SCENE_TEXTURE_ID::ENDING_FRAME_14, L"Resources\\Textures\\ending_frame\\frame_14_delay-0.1s.bmp");
	GraphicsHelper::InsertTexure(SCENE_TEXTURE_ID::ENDING_FRAME_15, L"Resources\\Textures\\ending_frame\\frame_15_delay-0.1s.bmp");
	GraphicsHelper::InsertTexure(SCENE_TEXTURE_ID::ENDING_FRAME_16, L"Resources\\Textures\\ending_frame\\frame_16_delay-0.1s.bmp");
	GraphicsHelper::InsertTexure(SCENE_TEXTURE_ID::ENDING_FRAME_17, L"Resources\\Textures\\ending_frame\\frame_17_delay-0.1s.bmp");
	GraphicsHelper::InsertTexure(SCENE_TEXTURE_ID::ENDING_FRAME_18, L"Resources\\Textures\\ending_frame\\frame_18_delay-0.1s.bmp");
	GraphicsHelper::InsertTexure(SCENE_TEXTURE_ID::ENDING_FRAME_19, L"Resources\\Textures\\ending_frame\\frame_19_delay-0.1s.bmp");
	GraphicsHelper::InsertTexure(SCENE_TEXTURE_ID::ENDING_FRAME_20, L"Resources\\Textures\\ending_frame\\frame_20_delay-0.1s.bmp");
	GraphicsHelper::InsertTexure(SCENE_TEXTURE_ID::ENDING_FRAME_21, L"Resources\\Textures\\ending_frame\\frame_21_delay-0.1s.bmp");
	GraphicsHelper::InsertTexure(SCENE_TEXTURE_ID::ENDING_FRAME_22, L"Resources\\Textures\\ending_frame\\frame_22_delay-0.1s.bmp");
	GraphicsHelper::InsertTexure(SCENE_TEXTURE_ID::ENDING_FRAME_23, L"Resources\\Textures\\ending_frame\\frame_23_delay-0.1s.bmp");
	GraphicsHelper::InsertTexure(SCENE_TEXTURE_ID::ENDING_FRAME_24, L"Resources\\Textures\\ending_frame\\frame_24_delay-0.1s.bmp");
	GraphicsHelper::InsertTexure(SCENE_TEXTURE_ID::ENDING_FRAME_25, L"Resources\\Textures\\ending_frame\\frame_25_delay-0.1s.bmp");
	GraphicsHelper::InsertTexure(SCENE_TEXTURE_ID::ENDING_FRAME_26, L"Resources\\Textures\\ending_frame\\frame_26_delay-0.1s.bmp");
	GraphicsHelper::InsertTexure(SCENE_TEXTURE_ID::ENDING_FRAME_27, L"Resources\\Textures\\ending_frame\\frame_27_delay-0.1s.bmp");
	GraphicsHelper::InsertTexure(SCENE_TEXTURE_ID::ENDING_FRAME_28, L"Resources\\Textures\\ending_frame\\frame_28_delay-0.1s.bmp");
	GraphicsHelper::InsertTexure(SCENE_TEXTURE_ID::ENDING_FRAME_29, L"Resources\\Textures\\ending_frame\\frame_29_delay-0.1s.bmp");
	GraphicsHelper::InsertTexure(SCENE_TEXTURE_ID::ENDING_FRAME_30, L"Resources\\Textures\\ending_frame\\frame_30_delay-0.1s.bmp");
	GraphicsHelper::InsertTexure(SCENE_TEXTURE_ID::ENDING_FRAME_31, L"Resources\\Textures\\ending_frame\\frame_31_delay-0.1s.bmp");
	GraphicsHelper::InsertTexure(SCENE_TEXTURE_ID::ENDING_FRAME_32, L"Resources\\Textures\\ending_frame\\frame_32_delay-0.1s.bmp");
	GraphicsHelper::InsertTexure(SCENE_TEXTURE_ID::ENDING_FRAME_33, L"Resources\\Textures\\ending_frame\\frame_33_delay-0.1s.bmp");
	GraphicsHelper::InsertTexure(SCENE_TEXTURE_ID::ENDING_FRAME_34, L"Resources\\Textures\\ending_frame\\frame_34_delay-0.1s.bmp");
	GraphicsHelper::InsertTexure(SCENE_TEXTURE_ID::ENDING_FRAME_35, L"Resources\\Textures\\ending_frame\\frame_35_delay-0.1s.bmp");
	GraphicsHelper::InsertTexure(SCENE_TEXTURE_ID::ENDING_FRAME_36, L"Resources\\Textures\\ending_frame\\frame_36_delay-0.1s.bmp");
	GraphicsHelper::InsertTexure(SCENE_TEXTURE_ID::ENDING_FRAME_37, L"Resources\\Textures\\ending_frame\\frame_37_delay-0.1s.bmp");
	GraphicsHelper::InsertTexure(SCENE_TEXTURE_ID::ENDING_FRAME_38, L"Resources\\Textures\\ending_frame\\frame_38_delay-0.1s.bmp");
	GraphicsHelper::InsertTexure(SCENE_TEXTURE_ID::ENDING_FRAME_39, L"Resources\\Textures\\ending_frame\\frame_39_delay-0.1s.bmp");
	GraphicsHelper::InsertTexure(SCENE_TEXTURE_ID::ENDING_FRAME_40, L"Resources\\Textures\\ending_frame\\frame_40_delay-0.1s.bmp");
	GraphicsHelper::InsertTexure(SCENE_TEXTURE_ID::ENDING_FRAME_41, L"Resources\\Textures\\ending_frame\\frame_41_delay-0.1s.bmp");
	GraphicsHelper::InsertTexure(SCENE_TEXTURE_ID::ENDING_FRAME_42, L"Resources\\Textures\\ending_frame\\frame_42_delay-0.1s.bmp");
	GraphicsHelper::InsertTexure(SCENE_TEXTURE_ID::ENDING_FRAME_43, L"Resources\\Textures\\ending_frame\\frame_43_delay-0.1s.bmp");
	GraphicsHelper::InsertTexure(SCENE_TEXTURE_ID::ENDING_FRAME_44, L"Resources\\Textures\\ending_frame\\frame_44_delay-0.1s.bmp");
	GraphicsHelper::InsertTexure(SCENE_TEXTURE_ID::ENDING_FRAME_45, L"Resources\\Textures\\ending_frame\\frame_45_delay-0.1s.bmp");
	GraphicsHelper::InsertTexure(SCENE_TEXTURE_ID::ENDING_FRAME_46, L"Resources\\Textures\\ending_frame\\frame_46_delay-0.1s.bmp");
	GraphicsHelper::InsertTexure(SCENE_TEXTURE_ID::ENDING_FRAME_47, L"Resources\\Textures\\ending_frame\\frame_47_delay-0.1s.bmp");
	GraphicsHelper::InsertTexure(SCENE_TEXTURE_ID::ENDING_FRAME_48, L"Resources\\Textures\\ending_frame\\frame_48_delay-0.1s.bmp");
	GraphicsHelper::InsertTexure(SCENE_TEXTURE_ID::ENDING_FRAME_49, L"Resources\\Textures\\ending_frame\\frame_49_delay-0.1s.bmp");
	GraphicsHelper::InsertTexure(SCENE_TEXTURE_ID::ENDING_FRAME_50, L"Resources\\Textures\\ending_frame\\frame_50_delay-0.1s.bmp");
	GraphicsHelper::InsertTexure(SCENE_TEXTURE_ID::ENDING_FRAME_51, L"Resources\\Textures\\ending_frame\\frame_51_delay-0.1s.bmp");

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


	GraphicsHelper::InsertAnimation(SCENE_ANIMATION_ID::ENDING_FRAME, 150,
		{
			{SCENE_SPRITE_ID::ENDING_FRAME_00,0},
			{SCENE_SPRITE_ID::ENDING_FRAME_01,0},
			{SCENE_SPRITE_ID::ENDING_FRAME_02,0},
			{SCENE_SPRITE_ID::ENDING_FRAME_03,0},
			{SCENE_SPRITE_ID::ENDING_FRAME_04,0},
			{SCENE_SPRITE_ID::ENDING_FRAME_05,0},
			{SCENE_SPRITE_ID::ENDING_FRAME_06,0},
			{SCENE_SPRITE_ID::ENDING_FRAME_07,0},
			{SCENE_SPRITE_ID::ENDING_FRAME_08,0},
			{SCENE_SPRITE_ID::ENDING_FRAME_09,0},
			{SCENE_SPRITE_ID::ENDING_FRAME_10,0},
			{SCENE_SPRITE_ID::ENDING_FRAME_11,0},
			{SCENE_SPRITE_ID::ENDING_FRAME_12,0},
			{SCENE_SPRITE_ID::ENDING_FRAME_13,0},
			{SCENE_SPRITE_ID::ENDING_FRAME_14,0},
			{SCENE_SPRITE_ID::ENDING_FRAME_15,0},
			{SCENE_SPRITE_ID::ENDING_FRAME_16,0},
			{SCENE_SPRITE_ID::ENDING_FRAME_17,0},
			{SCENE_SPRITE_ID::ENDING_FRAME_18,0},
			{SCENE_SPRITE_ID::ENDING_FRAME_19,0},
			{SCENE_SPRITE_ID::ENDING_FRAME_20,0},
			{SCENE_SPRITE_ID::ENDING_FRAME_21,0},
			{SCENE_SPRITE_ID::ENDING_FRAME_22,0},
			{SCENE_SPRITE_ID::ENDING_FRAME_23,0},
			{SCENE_SPRITE_ID::ENDING_FRAME_24,0},
			{SCENE_SPRITE_ID::ENDING_FRAME_25,0},
			{SCENE_SPRITE_ID::ENDING_FRAME_26,0},
			{SCENE_SPRITE_ID::ENDING_FRAME_27,0},
			{SCENE_SPRITE_ID::ENDING_FRAME_28,0},
			{SCENE_SPRITE_ID::ENDING_FRAME_29,0},
			{SCENE_SPRITE_ID::ENDING_FRAME_30,0},
			{SCENE_SPRITE_ID::ENDING_FRAME_31,0},
			{SCENE_SPRITE_ID::ENDING_FRAME_32,0},
			{SCENE_SPRITE_ID::ENDING_FRAME_33,0},
			{SCENE_SPRITE_ID::ENDING_FRAME_34,0},
			{SCENE_SPRITE_ID::ENDING_FRAME_35,0},
			{SCENE_SPRITE_ID::ENDING_FRAME_36,0},
			{SCENE_SPRITE_ID::ENDING_FRAME_37,0},
			{SCENE_SPRITE_ID::ENDING_FRAME_38,0},
			{SCENE_SPRITE_ID::ENDING_FRAME_39,0},
			{SCENE_SPRITE_ID::ENDING_FRAME_40,0},
			{SCENE_SPRITE_ID::ENDING_FRAME_41,0},
			{SCENE_SPRITE_ID::ENDING_FRAME_42,0},
			{SCENE_SPRITE_ID::ENDING_FRAME_43,0},
			{SCENE_SPRITE_ID::ENDING_FRAME_44,0},
			{SCENE_SPRITE_ID::ENDING_FRAME_45,0},
			{SCENE_SPRITE_ID::ENDING_FRAME_46,0},
			{SCENE_SPRITE_ID::ENDING_FRAME_47,0},
			{SCENE_SPRITE_ID::ENDING_FRAME_48,0},
			{SCENE_SPRITE_ID::ENDING_FRAME_49,0},
			{SCENE_SPRITE_ID::ENDING_FRAME_50,0},
			{SCENE_SPRITE_ID::ENDING_FRAME_51,0},
		});
	OutputDebugString(L"Scene Animations Loaded Successfully\n");
}
