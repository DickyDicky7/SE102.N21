#include "RockFly.h"

RockFly::RockFly(FLOAT begin, FLOAT end) : Entity(), HasTextures(), HasSprites()
{
	this->vx = 1.0f;
	this->vy = 1.0f;
	this->ax = 0.1f;
	this->ay = 0.1f;
	this->position.x = 0;
	this->position.y = 0;
	this->name = L"RockFly\n";

	this->movingDirection = DIRECTION::LEFT;
	this->xBegin = begin;
	this->xEnd = end;
}

RockFly::~RockFly()
{
}

void RockFly::Update()
{
	FLOAT x = GetX();

	if (x > xEnd || x < xBegin) vx = -vx;

	x += vx;
	SetX(x);
}

void RockFly::Render()
{
	this->w = this->currentFrameW;
	this->h = this->currentFrameH;
	SetAnimation(ROCK_FLY_ANIMATION_ID::NORMAL, GetPosition(), GetMovingDirection(), GetAngle());
}

void RockFly::HandleInput(Input& input)
{
	//handleInputState = state->HandleInput(*this, input);
}

void InsertSpriteRockFly(SPRITE_ID spriteId, INT left, INT top, INT right, INT bottom)
{
	// i write this function to shorten the fuction: GraphicsHelper
	GraphicsHelper::InsertSprite(spriteId, top, left, right, bottom, DIRECTION::RIGHT, ROCK_FLY_TEXTURE_ID::ROCK_FLY);
}

void RockFly::LoadSprites()
{
	if (HasSprites<RockFly>::hasBeenLoaded.value) return;
	HasSprites<RockFly>::hasBeenLoaded.value = true;

#pragma region Load Sprites

	// SPRITES

	InsertSpriteRockFly(ROCK_FLY_SPRITE_ID::NORMAL_01, 3, 2, 36, 33);

#pragma endregion Load Sprites

	OutputDebugString(L"RockFly Sprites Loaded Successfully\n");
}

void RockFly::LoadTextures()
{
	if (HasTextures<RockFly>::hasBeenLoaded.value) return;
	HasTextures<RockFly>::hasBeenLoaded.value = true;

	GraphicsHelper::InsertTexure(ROCK_FLY_TEXTURE_ID::ROCK_FLY, L"Resources\\Textures\\stage3_elements.png");

	OutputDebugString(L"RockFly Textures Loaded Successfully\n");
}

void RockFly::LoadAnimations()
{
	if (HasAnimations<RockFly>::hasBeenLoaded.value) return;
	HasAnimations<RockFly>::hasBeenLoaded.value = true;

#pragma region Load Animations

	GraphicsHelper::InsertAnimation(ROCK_FLY_ANIMATION_ID::NORMAL, 150,
		{
			{ROCK_FLY_SPRITE_ID::NORMAL_01,0},
		});
#pragma endregion Load Animations

	OutputDebugString(L"RockFly Animations Loaded Successfully\n");
}
