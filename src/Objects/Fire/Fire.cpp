#include "Fire.h"

Fire::Fire() : Entity(), HasTextures(), HasSprites(), HasAnimations()
{
	this->_vx = Constants::Objects::Fire::INITIAL_SPEED_X;
	this->_vy = Constants::Objects::Fire::INITIAL_SPEED_Y;
	this->_ax = Constants::Physics::DEFAULT_INITIAL_ACCELERATION_X;
	this->_ay = Constants::Physics::DEFAULT_INITIAL_ACCELERATION_Y;
	this->_position.x = 0;
	this->_position.y = 0;
	this->SetDebugName(L"Fire\n");

	this->_movingDirection = DIRECTION::LEFT;
	this->_distanceMove = Constants::Objects::Fire::MOVE_DISTANCE;
	this->_x0 = 0.0f;
	this->_hasCapturedOrigin = false;

	this->_enemyType = ENEMY_TYPE::NONE;
	this->_hitCounts = (std::numeric_limits<int>::max)();
}

Fire::~Fire()
{
}

void Fire::Update()
{
	float x = this->GetX();

	// Capture the patrol centre once, on the first update.
	if (!this->_hasCapturedOrigin)
	{
		this->_x0 = x;
		this->_hasCapturedOrigin = true;
	}

	if (this->GetMovingDirection() == DIRECTION::RIGHT)
	{
		if (x > this->_x0 + this->_distanceMove || x < this->_x0) this->_vx = -this->_vx;
	}
	else {
		if (x > this->_x0 || x < this->_x0 - this->_distanceMove) this->_vx = -this->_vx;
	}

	x += this->_vx;
	this->SetX(x);
}

void Fire::Render()
{
	this->_w = this->GetCurrentFrameW();
	this->_h = this->GetCurrentFrameH();
	this->SetAnimation(FIRE_ANIMATION_ID::NORMAL, this->GetPosition(), this->GetMovingDirection(), this->GetAngle());
}

void Fire::HandleInput(Input& input)
{
	//DeferState(_handleInputState, _state->HandleInput(*this, input));
}

void InsertSpriteFire(SPRITE_ID spriteId, int left, int top, int right, int bottom)
{
	// i write this function to shorten the function: GraphicsHelper
	GraphicsHelper::InsertSprite(spriteId, top, left, right, bottom, DIRECTION::RIGHT, FIRE_TEXTURE_ID::FIRE);
}

void Fire::LoadSprites()
{
	if (HasSprites<Fire>::_hasBeenLoaded) return;
	HasSprites<Fire>::_hasBeenLoaded = true;

#pragma region Load Sprites

	// SPRITES

	InsertSpriteFire(FIRE_SPRITE_ID::NORMAL_01, 0, 0, 16, 16);
	InsertSpriteFire(FIRE_SPRITE_ID::NORMAL_02, 16, 0, 32, 16);

#pragma endregion Load Sprites

	OutputDebugString(L"Fire Sprites Loaded Successfully\n");
}

void Fire::LoadTextures()
{
	if (HasTextures<Fire>::_hasBeenLoaded) return;
	HasTextures<Fire>::_hasBeenLoaded = true;

	GraphicsHelper::InsertTexture(FIRE_TEXTURE_ID::FIRE, L"Resources\\Textures\\Fire.bmp");

	OutputDebugString(L"Fire Textures Loaded Successfully\n");
}

void Fire::LoadAnimations()
{
	if (HasAnimations<Fire>::_hasBeenLoaded) return;
	HasAnimations<Fire>::_hasBeenLoaded = true;

#pragma region Load Animations

	GraphicsHelper::InsertAnimation(FIRE_ANIMATION_ID::NORMAL, Constants::Objects::Fire::ANIMATION_DELAY_MILLISECONDS,
		{
			{FIRE_SPRITE_ID::NORMAL_01,0},
			{FIRE_SPRITE_ID::NORMAL_02,0},
		});
#pragma endregion Load Animations

	OutputDebugString(L"Fire Animations Loaded Successfully\n");
}
