#pragma once
#include "RifleManStanding.h"

RifleManStanding::RifleManStanding() : Entity(), HasAnimations(), HasWeapons(new BulletEnemyState())
{
	this->vx = 1.0f;
	this->vy = 1.0f;
	this->ax = 0.1f;
	this->ay = 0.1f;
	this->position.x = 300;
	this->position.y = 200;

	this->movingDirection = DIRECTION::LEFT;
	this->name = L"RifleManStanding\n";

	this->updateState = NULL;
	this->state = new RifleManStandingNormalState();

	this->hitCounts = 1;
	this->enemyType = ENEMY_TYPE::HUMAN;

	this->firingRate = 0; 

	shootDelay = RILFE_MAN_STANDING_SHOOT_DELAY;
	shootTime = RILFE_MAN_STANDING_SHOOT_TIME;
	shootDelayPerBullet = RILFE_MAN_STANDING_SHOOT_DELAY_PER_BULLET;
}

RifleManStanding::~RifleManStanding()
{

}

const Bill* RifleManStanding::GetEnemyTarget()
{
	return Enemy::target;
}

void RifleManStanding::Update()
{
	if (Enemy::target->isDead)
		return;

	const float _shootingAngle = this->CalculateShootingAngle();

	float dx = (this->GetPosition().x) - (Enemy::target->GetPosition().x);
	float dy = -((this->GetPosition().y) - (Enemy::target->GetPosition().y));

	this->SetMovingDirection(DIRECTION::RIGHT);
	if (dx > 0)
	{
		this->SetMovingDirection(DIRECTION::LEFT);
	}

	if ((_shootingAngle >= 0 && _shootingAngle < 65) || (_shootingAngle < 0 && _shootingAngle >= -65))
	{
		updateState = new RifleManStandingAimUpState();
		if (dy < 0)
		{
			updateState = new RifleManStandingAimDownState();
		}
		return;
	}

	if (_shootingAngle >= 65 || _shootingAngle < -65 || _shootingAngle == 90)
	{
		updateState = new RifleManStandingNormalState();
		return;
	}

	updateState = NULL;
	return;
}

void RifleManStanding::Render()
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
}

FLOAT RifleManStanding::CalculateShootingAngle()
{
	// first and third quarter is positive, second and fourth quarter is negative, top and bottom is 0, left right is 90
	const Bill* bill = Enemy::target;

	float dx = (this->GetPosition().x) - (bill->GetPosition().x);
	float dy = -((this->GetPosition().y) - (bill->GetPosition().y));

	if (dy == 0)
	{
		return 90;
	}

	return D3DXToDegree(atan(dx / dy));
}

void RifleManStanding::HandleInput(Input& input)
{

}

void RifleManStanding::LoadTextures()
{
	RifleMan::LoadTextures();
}

void RifleManStanding::LoadSprites()
{
	if (HasSprites<RifleManStanding>::hasBeenLoaded.value) {
		return;
	}
	HasSprites<RifleManStanding>::hasBeenLoaded.value = true;

	GraphicsHelper::InsertSprite(RIFLE_MAN_SPRITE_ID::SHOOT_NORMAL_01, 0, 0, 23, 38, DIRECTION::LEFT, RIFLE_MAN_TEXTURE_ID::RIFLE_MAN);
	GraphicsHelper::InsertSprite(RIFLE_MAN_SPRITE_ID::SHOOT_NORMAL_02, 0, 26, 49, 38, DIRECTION::LEFT, RIFLE_MAN_TEXTURE_ID::RIFLE_MAN);
	GraphicsHelper::InsertSprite(RIFLE_MAN_SPRITE_ID::SHOOT_UP_01, 0, 52, 69, 38, DIRECTION::LEFT, RIFLE_MAN_TEXTURE_ID::RIFLE_MAN);
	GraphicsHelper::InsertSprite(RIFLE_MAN_SPRITE_ID::SHOOT_UP_02, 0, 72, 89, 38, DIRECTION::LEFT, RIFLE_MAN_TEXTURE_ID::RIFLE_MAN);
	GraphicsHelper::InsertSprite(RIFLE_MAN_SPRITE_ID::SHOOT_DOWN, 0, 92, 115, 38, DIRECTION::LEFT, RIFLE_MAN_TEXTURE_ID::RIFLE_MAN);
}

void RifleManStanding::LoadAnimations()
{
	if (HasAnimations<RifleManStanding>::hasBeenLoaded.value) {
		return;
	}
	HasAnimations<RifleManStanding>::hasBeenLoaded.value = true;

	GraphicsHelper::InsertAnimation(RIFLE_MAN_ANIMATION_ID::SHOOT_NORMAL, 165,
		{
			{RIFLE_MAN_SPRITE_ID::SHOOT_NORMAL_01, 0},
			{RIFLE_MAN_SPRITE_ID::SHOOT_NORMAL_02, 0},
		});

	GraphicsHelper::InsertAnimation(RIFLE_MAN_ANIMATION_ID::SHOOT_UP, 165,
		{
			{RIFLE_MAN_SPRITE_ID::SHOOT_UP_01, 0},
			{RIFLE_MAN_SPRITE_ID::SHOOT_UP_02, 0},
		});

	GraphicsHelper::InsertAnimation(RIFLE_MAN_ANIMATION_ID::SHOOT_DOWN, 165,
		{
			{RIFLE_MAN_SPRITE_ID::SHOOT_DOWN, 0},
		});
}

void RifleManStanding::Fire()
{
	if (Enemy::target->isDead)
	{
		return;
	}
}

void RifleManStanding::CustomFire(FLOAT x, FLOAT y, FLOAT angle, FLOAT vx, FLOAT vy, FLOAT ax, FLOAT ay, DIRECTION movingDirection)
{
	if (Enemy::target->isDead)
	{
		return;
	}

	HasWeapons::Fire(x, y, angle, vx, vy, ax, ay, movingDirection);
}