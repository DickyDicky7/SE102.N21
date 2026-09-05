#include "RockFly.h"

RockFly::RockFly(float xBegin, float xEnd) : Entity(), HasTextures(), HasSprites()
{
	this->_vx = Constants::Physics::DEFAULT_INITIAL_VELOCITY_X;
	this->_vy = Constants::Physics::DEFAULT_INITIAL_VELOCITY_Y;
	this->_ax = Constants::Physics::DEFAULT_INITIAL_ACCELERATION_X;
	this->_ay = Constants::Physics::DEFAULT_INITIAL_ACCELERATION_Y;
	this->_position.x = 0;
	this->_position.y = 0;
	this->SetDebugName(L"RockFly\n");

	this->_movingDirection = DIRECTION::LEFT;
	this->_xBegin = xBegin;
	this->_xEnd = xEnd;
}

RockFly::~RockFly()
{
	this->_xBegin = 0.0f;
	this->_xEnd = 0.0f;
}

void RockFly::Update()
{
	float x = this->GetX();

	if (x > this->_xEnd || x < this->_xBegin) this->_vx = -this->_vx;

	x += this->_vx;
	this->SetX(x);
}

void RockFly::Render()
{
	this->_w = this->GetCurrentFrameW();
	this->_h = this->GetCurrentFrameH();
	this->_h -= Constants::Objects::RockFly::HITBOX_HEIGHT_OFFSET;
	this->SetAnimation(ROCK_FLY_ANIMATION_ID::NORMAL, this->GetPosition(), this->GetMovingDirection(), this->GetAngle());
}

void RockFly::HandleInput(Input& input)
{
	//DeferState(_handleInputState, _state->HandleInput(*this, input));
}

void InsertSpriteRockFly(SPRITE_ID spriteId, int left, int top, int right, int bottom)
{
	// i write this function to shorten the function: GraphicsHelper
	GraphicsHelper::InsertSprite(spriteId, top, left, right, bottom, DIRECTION::RIGHT, ROCK_FLY_TEXTURE_ID::ROCK_FLY);
}

void RockFly::LoadSprites()
{
	if (HasSprites<RockFly>::_hasBeenLoaded) return;
	HasSprites<RockFly>::_hasBeenLoaded = true;

#pragma region Load Sprites

	// SPRITES

	InsertSpriteRockFly(ROCK_FLY_SPRITE_ID::NORMAL_01, 3, 2, 36, 33);

#pragma endregion Load Sprites

	OutputDebugString(L"RockFly Sprites Loaded Successfully\n");
}

void RockFly::LoadTextures()
{
	if (HasTextures<RockFly>::_hasBeenLoaded) return;
	HasTextures<RockFly>::_hasBeenLoaded = true;

	GraphicsHelper::InsertTexture(ROCK_FLY_TEXTURE_ID::ROCK_FLY, L"Resources\\Textures\\Stage3Elements.bmp");

	OutputDebugString(L"RockFly Textures Loaded Successfully\n");
}

void RockFly::LoadAnimations()
{
	if (HasAnimations<RockFly>::_hasBeenLoaded) return;
	HasAnimations<RockFly>::_hasBeenLoaded = true;

#pragma region Load Animations

	GraphicsHelper::InsertAnimation(ROCK_FLY_ANIMATION_ID::NORMAL, Constants::Objects::RockFly::ANIMATION_DELAY_MILLISECONDS,
		{
			{ROCK_FLY_SPRITE_ID::NORMAL_01,0},
		});
#pragma endregion Load Animations

	OutputDebugString(L"RockFly Animations Loaded Successfully\n");
}
