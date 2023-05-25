#include "Fire.h"

Fire::Fire() : Entity(), HasTextures(), HasSprites(), HasAnimations()
{
	this->vx = 1.0f;
	this->vy = 1.0f;
	this->ax = 0.1f;
	this->ay = 0.1f;
	this->position.x = 0;
	this->position.y = 0;
	this->name = L"Fire\n";

	this->movingDirection = DIRECTION::LEFT;
	this->distanceMove = 110.0f;
	this->x0 = NULL;
}

Fire::~Fire()
{
	distanceMove = NULL;
	x0 = NULL;
}

void Fire::Update()
{
	FLOAT x = GetX();
	if (x0 == NULL) x0 = x;

	if (GetMovingDirection() == DIRECTION::RIGHT) 
	{
		if (x > x0 + distanceMove || x < x0) vx = -vx;
	}
	else {
		if (x > x0 || x < x0 - distanceMove) vx = -vx;
	}

	x += vx;
	SetX(x);
}

void Fire::Render()
{
	this->w = this->currentFrameW;
	this->h = this->currentFrameH;
	SetAnimation(FIRE_ANIMATION_ID::NORMAL, GetPosition(), GetMovingDirection(), GetAngle());
}

void Fire::HandleInput(Input& input)
{
	//handleInputState = state->HandleInput(*this, input);
}

void InsertSpriteFire(SPRITE_ID spriteId, INT left, INT top, INT right, INT bottom)
{
	// i write this function to shorten the fuction: GraphicsHelper
	GraphicsHelper::InsertSprite(spriteId, top, left, right, bottom, DIRECTION::RIGHT, FIRE_TEXTURE_ID::FIRE);
}

void Fire::LoadSprites()
{
	if (HasSprites<Fire>::hasBeenLoaded.value) return;
	HasSprites<Fire>::hasBeenLoaded.value = true;

#pragma region Load Sprites

	// SPRITES

	InsertSpriteFire(FIRE_SPRITE_ID::NORMAL_01, 0, 0, 16, 16);
	InsertSpriteFire(FIRE_SPRITE_ID::NORMAL_02, 16, 0, 32, 16);

#pragma endregion Load Sprites

	OutputDebugString(L"Fire Sprites Loaded Successfully\n");
}

void Fire::LoadTextures()
{
	if (HasTextures<Fire>::hasBeenLoaded.value) return;
	HasTextures<Fire>::hasBeenLoaded.value = true;

	GraphicsHelper::InsertTexure(FIRE_TEXTURE_ID::FIRE, L"Resources\\Textures\\fire.png");

	OutputDebugString(L"Fire Textures Loaded Successfully\n");
}

void Fire::LoadAnimations()
{
	if (HasAnimations<Fire>::hasBeenLoaded.value) return;
	HasAnimations<Fire>::hasBeenLoaded.value = true;

#pragma region Load Animations

	GraphicsHelper::InsertAnimation(FIRE_ANIMATION_ID::NORMAL, 150,
		{
			{FIRE_SPRITE_ID::NORMAL_01,0},
			{FIRE_SPRITE_ID::NORMAL_02,0},
		});
#pragma endregion Load Animations

	OutputDebugString(L"Fire Animations Loaded Successfully\n");
}
