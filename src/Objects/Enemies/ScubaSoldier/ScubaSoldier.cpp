#include "ScubaSoldier.h"

ScubaSoldier::ScubaSoldier() : Entity(), HasTextures(), HasSprites(), HasAnimations(), HasWeapons(new BulletScubaSoldierState(0.0f, 0.0f))
{
	this->_vx = Constants::Physics::DEFAULT_INITIAL_VELOCITY_X;
	this->_vy = Constants::Physics::DEFAULT_INITIAL_VELOCITY_Y;
	this->_ax = Constants::Physics::DEFAULT_INITIAL_ACCELERATION_X;
	this->_ay = Constants::Physics::DEFAULT_INITIAL_ACCELERATION_Y;
	this->_position.x = Constants::Enemies::ScubaSoldier::DEFAULT_SPAWN_X;
	this->_position.y = Constants::Enemies::ScubaSoldier::DEFAULT_SPAWN_Y;
	this->SetDebugName(L"ScubaSoldier\n");

	this->_updateState = nullptr;
	this->_handleInputState = nullptr;
	// set direction default is right
	this->_movingDirection = DIRECTION::RIGHT;
	// set state begin is run
	this->_state = new ScubaSoldierHiddenState();

	this->_billAngle = Constants::Enemies::ScubaSoldier::INITIAL_BILL_ANGLE_DEGREES;

	this->_hitCounts = Constants::Enemies::ScubaSoldier::HEALTH_POINTS;
	this->_enemyType = ENEMY_TYPE::HUMAN;

	this->_firingRate = Constants::Enemies::ScubaSoldier::FIRING_RATE_MILLISECONDS;
}

ScubaSoldier::~ScubaSoldier()
{
	Destroy(this->_state);
	Destroy(this->_updateState);
	Destroy(this->_handleInputState);
}

void ScubaSoldier::Update()
{
	this->CalculateBillAngle();
	if (this->_billAngle >= Constants::Enemies::ScubaSoldier::DETECT_ANGLE_MAX_DEGREES || this->_billAngle < Constants::Enemies::ScubaSoldier::DETECT_ANGLE_MIN_DEGREES)
	{
		if (this->_state && this->_state->IsHidden())
		{
			ChangeState(this->_state, new ScubaSoldierShootingState(), this);
		}
	}

	DeferState(this->_updateState, this->_state->Update(*this));

	ApplyDeferredState(this->_state, this->_updateState, this);
	ApplyDeferredState(this->_state, this->_handleInputState, this);
}

void ScubaSoldier::Fire()
{
	this->HasWeapons::Fire(this->GetX(), this->GetT(), 0.0f, 0.0f, Constants::Enemies::ScubaSoldier::BULLET_SPEED_Y, 0.0f, Constants::Enemies::ScubaSoldier::BULLET_ACCELERATION_Y, this->_movingDirection);
}

void ScubaSoldier::CalculateBillAngle()
{
	this->_billAngle = this->CalculateTargetAngle(this);
}

void ScubaSoldier::Render()
{
	this->_state->Render(*this);
	this->_w = this->GetCurrentFrameW();
	this->_h = this->GetCurrentFrameH();
}

void ScubaSoldier::HandleInput(Input& input)
{
	DeferState(this->_handleInputState, this->_state->HandleInput(*this, input));
}

void InsertSpriteScuba(SPRITE_ID spriteId, int left, int top, int right, int bottom)
{
	// i write this function to shorten the function: GraphicsHelper
	GraphicsHelper::InsertSprite(spriteId, top, left, right, bottom, DIRECTION::RIGHT, SCUBA_SOLDIER_TEXTURE_ID::SCUBA_SOLDIER_01);
}

void ScubaSoldier::LoadSprites()
{
	if (HasSprites<ScubaSoldier>::_hasBeenLoaded) return;
	HasSprites<ScubaSoldier>::_hasBeenLoaded = true;

#pragma region Load Sprites

	// SPRITES

	InsertSpriteScuba(SCUBA_SOLDIER_SPRITE_ID::HIDDEN_01, 2, 0, 17, 30);

	InsertSpriteScuba(SCUBA_SOLDIER_SPRITE_ID::SHOOTING_01, 20, 0, 35, 30);

#pragma endregion Load Sprites

	OutputDebugString(L"ScubaSoldier Sprites Loaded Successfully\n");
}

void ScubaSoldier::LoadTextures()
{
	if (HasTextures<ScubaSoldier>::_hasBeenLoaded) return;
	HasTextures<ScubaSoldier>::_hasBeenLoaded = true;

	GraphicsHelper::InsertTexture(SCUBA_SOLDIER_TEXTURE_ID::SCUBA_SOLDIER_01, L"Resources\\Textures\\ScubaSoldier.bmp");

	OutputDebugString(L"ScubaSoldier Textures Loaded Successfully\n");
}

void ScubaSoldier::LoadAnimations()
{
	if (HasAnimations<ScubaSoldier>::_hasBeenLoaded) return;
	HasAnimations<ScubaSoldier>::_hasBeenLoaded = true;

#pragma region Load Animations

	GraphicsHelper::InsertAnimation(SCUBA_SOLDIER_ANIMATION_ID::HIDDEN, Constants::Enemies::ScubaSoldier::ANIMATION_DELAY_MILLISECONDS,
		{
			{SCUBA_SOLDIER_SPRITE_ID::HIDDEN_01,0},
		});
	GraphicsHelper::InsertAnimation(SCUBA_SOLDIER_ANIMATION_ID::SHOOTING, Constants::Enemies::ScubaSoldier::ANIMATION_DELAY_MILLISECONDS,
		{
			{SCUBA_SOLDIER_SPRITE_ID::SHOOTING_01,0},
		});

#pragma endregion Load Animations

	OutputDebugString(L"ScubaSoldier Animations Loaded Successfully\n");
}
