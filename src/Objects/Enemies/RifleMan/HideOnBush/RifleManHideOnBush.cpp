#include "RifleManHideOnBush.h"

RifleManHideOnBush::RifleManHideOnBush() : Entity(), HasAnimations(), HasWeapons(new BulletEnemyState())
{
	this->_vx = Constants::Physics::DEFAULT_INITIAL_VELOCITY_X;
	this->_vy = Constants::Physics::DEFAULT_INITIAL_VELOCITY_Y;
	this->_ax = Constants::Physics::DEFAULT_INITIAL_ACCELERATION_X;
	this->_ay = Constants::Physics::DEFAULT_INITIAL_ACCELERATION_Y;
	this->_position.x = Constants::Screen::WIDTH / Constants::Enemies::RifleMan::HIDE_ON_BUSH_SPAWN_DIVISOR_X;
	this->_position.y = Constants::Screen::HEIGHT / Constants::Enemies::RifleMan::HIDE_ON_BUSH_SPAWN_DIVISOR_Y - Constants::Enemies::RifleMan::HIDE_ON_BUSH_SPAWN_OFFSET_Y;

	this->_movingDirection = DIRECTION::LEFT;
	this->SetDebugName(L"RifleManHideOnBush\n");

	this->_updateState = nullptr;
	this->_state = new RifleManHideOnBushHidingState();

	this->_hitCounts = Constants::Enemies::RifleMan::HEALTH_POINTS;
	this->_enemyType = ENEMY_TYPE::HUMAN;

	this->_firingRate = 0;
}

RifleManHideOnBush::~RifleManHideOnBush()
{
	Destroy(this->_state);
	Destroy(this->_updateState);
}

const Bill* RifleManHideOnBush::GetEnemyTarget()
{
	return this->_target;
}

void RifleManHideOnBush::Update()
{
	DeferState(this->_updateState, this->_state->Update(*this));

	ApplyDeferredState(this->_state, this->_updateState, this);
}

void RifleManHideOnBush::Render()
{
	this->_state->Render(*this);
	this->_w = this->GetCurrentFrameW();
	this->_h = this->GetCurrentFrameH();
}

void RifleManHideOnBush::HandleInput(Input& input)
{

}

void RifleManHideOnBush::LoadTextures()
{
	RifleMan::LoadTextures();
}

void RifleManHideOnBush::LoadSprites()
{
	if (HasSprites<RifleManHideOnBush>::_hasBeenLoaded) {
		return;
	}
	HasSprites<RifleManHideOnBush>::_hasBeenLoaded = true;

	GraphicsHelper::InsertSprite(RIFLE_MAN_SPRITE_ID::APPEAR_01, 0, 128, 150, 38, DIRECTION::LEFT, RIFLE_MAN_TEXTURE_ID::RIFLE_MAN);
	GraphicsHelper::InsertSprite(RIFLE_MAN_SPRITE_ID::APPEAR_02, 0, 153, 176, 38, DIRECTION::LEFT, RIFLE_MAN_TEXTURE_ID::RIFLE_MAN);
	GraphicsHelper::InsertSprite(RIFLE_MAN_SPRITE_ID::HIDE, 0, 118, 125, 38, DIRECTION::LEFT, RIFLE_MAN_TEXTURE_ID::RIFLE_MAN);
}

void RifleManHideOnBush::LoadAnimations()
{
	if (HasAnimations<RifleManHideOnBush>::_hasBeenLoaded) {
		return;
	}
	HasAnimations<RifleManHideOnBush>::_hasBeenLoaded = true;

	GraphicsHelper::InsertAnimation(RIFLE_MAN_ANIMATION_ID::APPEAR, Constants::Enemies::RifleMan::ANIMATION_HIDE_ON_BUSH_DELAY_MILLISECONDS,
		{
			{RIFLE_MAN_SPRITE_ID::HIDE, 0},
			{RIFLE_MAN_SPRITE_ID::APPEAR_01, 0},
			{RIFLE_MAN_SPRITE_ID::APPEAR_02, 0},
		});

	GraphicsHelper::InsertAnimation(RIFLE_MAN_ANIMATION_ID::HIDE, Constants::Enemies::RifleMan::ANIMATION_HIDE_ON_BUSH_DELAY_MILLISECONDS,
		{
			{RIFLE_MAN_SPRITE_ID::APPEAR_02, 0},
			{RIFLE_MAN_SPRITE_ID::APPEAR_01, 0},
			{RIFLE_MAN_SPRITE_ID::HIDE, 0},
		});

	GraphicsHelper::InsertAnimation(RIFLE_MAN_ANIMATION_ID::HIDDING, Constants::Enemies::RifleMan::ANIMATION_HIDE_ON_BUSH_DELAY_MILLISECONDS,
		{
			{RIFLE_MAN_SPRITE_ID::HIDE, 0},
		});

	GraphicsHelper::InsertAnimation(RIFLE_MAN_ANIMATION_ID::STANDING, Constants::Enemies::RifleMan::ANIMATION_HIDE_ON_BUSH_DELAY_MILLISECONDS,
		{
			{RIFLE_MAN_SPRITE_ID::APPEAR_02, 0},
		});
}

void RifleManHideOnBush::Fire()
{
	if (!Enemy::_target || Enemy::_target->IsDead())
	{
		return;
	}
}

void RifleManHideOnBush::CustomFire(float x, float y, float angle, float vx, float vy, float ax, float ay, DIRECTION movingDirection)
{
	if (!Enemy::_target || Enemy::_target->IsDead())
	{
		return;
	}

	HasWeapons::Fire(x, y, angle, vx, vy, ax, ay, movingDirection);
}
