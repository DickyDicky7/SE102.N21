#include "RockFall.h"

RockFall::RockFall() : Entity(), HasTextures(), HasSprites(), HasAnimations()
{
	this->_timedelayToFall = 70;

	this->vx = 1.0f;
	this->vy = 1.0f;
	this->ax = 0.1f;
	this->ay = 0.1f;
	this->position.x = 0;
	this->position.y = 0;
	this->name = L"RockFall\n";

	this->updateState = NULL;
	this->handleInputState = NULL;
	// set direction default is right
	this->movingDirection = DIRECTION::RIGHT;
	// set state begin is run
	this->state = new RockFallNormalState();
}

RockFall::~RockFall()
{
	Destroy(state);
	Destroy(updateState);
	Destroy(handleInputState);

	_timedelayToFall = NULL;
}

void RockFall::Update()
{
	updateState = state->Update(*this);
}

void RockFall::Render()
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

void RockFall::HandleInput(Input& input)
{
	handleInputState = state->HandleInput(*this, input);
}

void InsertSpriteRockFall(SPRITE_ID spriteId, INT left, INT top, INT right, INT bottom)
{
	// i write this function to shorten the fuction: GraphicsHelper
	GraphicsHelper::InsertSprite(spriteId, top, left, right, bottom, DIRECTION::RIGHT, ROCK_FALL_TEXTURE_ID::ROCK_FALL);
}

void RockFall::LoadSprites()
{
	if (HasSprites<RockFall>::hasBeenLoaded.value) return;
	HasSprites<RockFall>::hasBeenLoaded.value = true;

#pragma region Load Sprites

	// SPRITES

	InsertSpriteRockFall(ROCK_FALL_SPRITE_ID::NORMAL_01, 5, 2, 28, 28);

	InsertSpriteRockFall(ROCK_FALL_SPRITE_ID::FALL_01, 35, 4, 61, 27);
	InsertSpriteRockFall(ROCK_FALL_SPRITE_ID::FALL_02, 69, 2, 92, 28);
	InsertSpriteRockFall(ROCK_FALL_SPRITE_ID::FALL_03, 99, 4, 125, 27);

#pragma endregion Load Sprites

	OutputDebugString(L"RockFall Sprites Loaded Successfully\n");
}

void RockFall::LoadTextures()
{
	if (HasTextures<RockFall>::hasBeenLoaded.value) return;
	HasTextures<RockFall>::hasBeenLoaded.value = true;

	GraphicsHelper::InsertTexure(ROCK_FALL_TEXTURE_ID::ROCK_FALL, L"Resources\\Textures\\rockfall.png");

	OutputDebugString(L"RockFall Textures Loaded Successfully\n");
}

void RockFall::LoadAnimations()
{
	if (HasAnimations<RockFall>::hasBeenLoaded.value) return;
	HasAnimations<RockFall>::hasBeenLoaded.value = true;

#pragma region Load Animations

	GraphicsHelper::InsertAnimation(ROCK_FALL_ANIMATION_ID::NORMAL, 150,
		{
			{ROCK_FALL_SPRITE_ID::NORMAL_01,0},
		});
	GraphicsHelper::InsertAnimation(ROCK_FALL_ANIMATION_ID::FALL, 150,
		{
			{ROCK_FALL_SPRITE_ID::FALL_01,0},
			{ROCK_FALL_SPRITE_ID::FALL_02,0},
			{ROCK_FALL_SPRITE_ID::FALL_03,0},
		});
#pragma endregion Load Animations

	OutputDebugString(L"RockFall Animations Loaded Successfully\n");
}
