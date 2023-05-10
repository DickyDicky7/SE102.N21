#include "BossStage3.h"
// O: open
// P: close

BossStage3::BossStage3() : Entity(), HasTextures(), HasSprites(), HasAnimations()
{
	Enemy::self = this;

	this->vx = 1.0f;
	this->vy = 1.0f;
	this->ax = 0.1f;
	this->ay = 0.1f;
	this->position.x = 200;
	this->position.y = 200;
	this->name = L"BossStage3\n";

	this->updateState = NULL;
	this->handleInputState = NULL;
	// set direction default is right
	this->movingDirection = DIRECTION::RIGHT;
	// set state begin is run
	this->state = new BossStage3CloseState();
}

BossStage3::~BossStage3()
{
}

void BossStage3::Update()
{
	updateState = state->Update(*this);
}

void BossStage3::Render()
{
	state->Render(*this);
	this->w = this->currentFrameW;
	this->h = this->currentFrameH;

	if (updateState)
	{
		state->Exit(*this);
		delete state;
		state = updateState;
		state->Enter(*this);
		updateState = NULL;
	}
	if (handleInputState)
	{
		state->Exit(*this);
		delete state;
		state = handleInputState;
		state->Enter(*this);
		handleInputState = NULL;
	}
}

void BossStage3::HandleInput(Input& input)
{
	handleInputState = state->HandleInput(*this, input);
}

void InsertSpriteBoss3(SPRITE_ID spriteId, INT left, INT top, INT right, INT bottom)
{
	// i write this function to shorten the fuction: GraphicsHelper
	GraphicsHelper::InsertSprite(spriteId, top, left, right, bottom, DIRECTION::RIGHT, BOSS_STAGE_3_TEXTURE_ID::BOSS_STAGE_3);
}

void BossStage3::LoadSprites()
{
	if (HasSprites<BossStage3>::hasBeenLoaded.value) return;
	HasSprites<BossStage3>::hasBeenLoaded.value = true;

#pragma region Load Sprites

	// SPRITES

	InsertSpriteBoss3(BOSS_STAGE_3_SPRITE_ID::MIDDLE_01, 6, 5, 96 + 6, 72 + 5);
	InsertSpriteBoss3(BOSS_STAGE_3_SPRITE_ID::MIDDLE_02, 112, 5, 96 + 112, 72 + 5);

	InsertSpriteBoss3(BOSS_STAGE_3_SPRITE_ID::OPEN_1, 218, 5, 96 + 218, 81 + 5);
	InsertSpriteBoss3(BOSS_STAGE_3_SPRITE_ID::OPEN_2, 324, 5, 96 + 324, 81 + 5);
	InsertSpriteBoss3(BOSS_STAGE_3_SPRITE_ID::OPEN_3, 430, 5, 96 + 430, 81 + 5);

	InsertSpriteBoss3(BOSS_STAGE_3_SPRITE_ID::CLOSE_1, 6, 96, 96 + 6, 62 + 96);
	InsertSpriteBoss3(BOSS_STAGE_3_SPRITE_ID::CLOSE_2, 112, 96, 96 + 112, 62 + 96);
	InsertSpriteBoss3(BOSS_STAGE_3_SPRITE_ID::CLOSE_3, 219, 96, 96 + 219, 62 + 96);

#pragma endregion Load Sprites

	OutputDebugString(L"BossStage3 Sprites Loaded Successfully\n");
}

void BossStage3::LoadTextures()
{
	if (HasTextures<BossStage3>::hasBeenLoaded.value) return;
	HasTextures<BossStage3>::hasBeenLoaded.value = true;

	GraphicsHelper::InsertTexure(BOSS_STAGE_3_TEXTURE_ID::BOSS_STAGE_3, L"Resources\\Textures\\BossStage3.bmp");

	OutputDebugString(L"BossStage3 Textures Loaded Successfully\n");
}

void BossStage3::LoadAnimations()
{
	if (HasAnimations<BossStage3>::hasBeenLoaded.value) return;
	HasAnimations<BossStage3>::hasBeenLoaded.value = true;

#pragma region Load Animations

	GraphicsHelper::InsertAnimation(BOSS_STAGE_3_ANIMATION_ID::MIDDLE, 150,
		{
			{BOSS_STAGE_3_SPRITE_ID::MIDDLE_01,0},
			{BOSS_STAGE_3_SPRITE_ID::MIDDLE_02,0},
		});

	GraphicsHelper::InsertAnimation(BOSS_STAGE_3_ANIMATION_ID::OPEN, 150,
		{
			{BOSS_STAGE_3_SPRITE_ID::OPEN_1,0},
			{BOSS_STAGE_3_SPRITE_ID::OPEN_2,0},
			{BOSS_STAGE_3_SPRITE_ID::OPEN_3,0},
		});
	GraphicsHelper::InsertAnimation(BOSS_STAGE_3_ANIMATION_ID::CLOSE, 150,
		{
			{BOSS_STAGE_3_SPRITE_ID::CLOSE_1,0},
			{BOSS_STAGE_3_SPRITE_ID::CLOSE_2,0},
			{BOSS_STAGE_3_SPRITE_ID::CLOSE_3,0},
		});
#pragma endregion Load Animations

	OutputDebugString(L"BossStage3 Animations Loaded Successfully\n");
}
