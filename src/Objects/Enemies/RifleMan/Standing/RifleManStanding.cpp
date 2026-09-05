#include "RifleManStanding.h"

RifleManStanding::RifleManStanding() : Entity(), HasAnimations(), HasWeapons(new BulletEnemyState())
{
	this->_vx = Constants::Physics::DEFAULT_INITIAL_VELOCITY_X;
	this->_vy = Constants::Physics::DEFAULT_INITIAL_VELOCITY_Y;
	this->_ax = Constants::Physics::DEFAULT_INITIAL_ACCELERATION_X;
	this->_ay = Constants::Physics::DEFAULT_INITIAL_ACCELERATION_Y;
	this->_position.x = Constants::Enemies::RifleMan::STANDING_DEFAULT_SPAWN_X;
	this->_position.y = Constants::Enemies::RifleMan::STANDING_DEFAULT_SPAWN_Y;

	this->_movingDirection = DIRECTION::LEFT;
	this->SetDebugName(L"RifleManStanding\n");

	this->_updateState = nullptr;
	this->_state = new RifleManStandingNormalState();

	this->_hitCounts = Constants::Enemies::RifleMan::HEALTH_POINTS;
	this->_enemyType = ENEMY_TYPE::HUMAN;

	this->_firingRate = 0;

	this->ResetBurst();
}

RifleManStanding::~RifleManStanding()
{
	Destroy(this->_state);
	Destroy(this->_updateState);
}

const Bill* RifleManStanding::GetEnemyTarget()
{
	return this->_target;
}

void RifleManStanding::Update()
{
	// Single exit, so the queue drained at the bottom is reached on every path.
	// The dead-target case returns without touching it, which leaves the rifleman
	// frozen in whatever state it was aiming from - as it did before.
	if (!this->_target || this->_target->IsDead())
		return;

	const float shootingAngle = this->CalculateShootingAngle();

	float dx = (this->GetPosition().x) - (this->_target->GetPosition().x);
	float dy = -((this->GetPosition().y) - (this->_target->GetPosition().y));

	this->SetMovingDirection(DIRECTION::RIGHT);
	if (dx > 0)
	{
		this->SetMovingDirection(DIRECTION::LEFT);
	}

	if ((shootingAngle >= 0 && shootingAngle < Constants::Enemies::RifleMan::SHOOTING_ANGLE_THRESHOLD_DEGREES) || (shootingAngle < 0 && shootingAngle >= -Constants::Enemies::RifleMan::SHOOTING_ANGLE_THRESHOLD_DEGREES))
	{
		if (dy < 0)
		{
			DeferState(this->_updateState, static_cast<RifleManStandingState*>(new RifleManStandingAimDownState()));
		}
		else
		{
			DeferState(this->_updateState, static_cast<RifleManStandingState*>(new RifleManStandingAimUpState()));
		}
	}
	else
	if (shootingAngle >= Constants::Enemies::RifleMan::SHOOTING_ANGLE_THRESHOLD_DEGREES || shootingAngle < -Constants::Enemies::RifleMan::SHOOTING_ANGLE_THRESHOLD_DEGREES || shootingAngle == Constants::Enemies::RifleMan::STRAIGHT_UP_ANGLE_DEGREES)
	{
		DeferState(this->_updateState, static_cast<RifleManStandingState*>(new RifleManStandingNormalState()));
	}
	else
	{
		Destroy(this->_updateState);
	}

	ApplyDeferredState(this->_state, this->_updateState, this);
}

void RifleManStanding::Render()
{
	this->_state->Render(*this);
	this->_w = this->GetCurrentFrameW();
	this->_h = this->GetCurrentFrameH();
}

float RifleManStanding::CalculateShootingAngle() const
{
	return this->Enemy<Bill>::CalculateShootingAngle(this);
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
	if (HasSprites<RifleManStanding>::_hasBeenLoaded) {
		return;
	}
	HasSprites<RifleManStanding>::_hasBeenLoaded = true;

	GraphicsHelper::InsertSprite(RIFLE_MAN_SPRITE_ID::SHOOT_NORMAL_01, 0, 0, 23, 38, DIRECTION::LEFT, RIFLE_MAN_TEXTURE_ID::RIFLE_MAN);
	GraphicsHelper::InsertSprite(RIFLE_MAN_SPRITE_ID::SHOOT_NORMAL_02, 0, 26, 49, 38, DIRECTION::LEFT, RIFLE_MAN_TEXTURE_ID::RIFLE_MAN);
	GraphicsHelper::InsertSprite(RIFLE_MAN_SPRITE_ID::SHOOT_UP_01, 0, 52, 69, 38, DIRECTION::LEFT, RIFLE_MAN_TEXTURE_ID::RIFLE_MAN);
	GraphicsHelper::InsertSprite(RIFLE_MAN_SPRITE_ID::SHOOT_UP_02, 0, 72, 89, 38, DIRECTION::LEFT, RIFLE_MAN_TEXTURE_ID::RIFLE_MAN);
	GraphicsHelper::InsertSprite(RIFLE_MAN_SPRITE_ID::SHOOT_DOWN, 0, 92, 115, 38, DIRECTION::LEFT, RIFLE_MAN_TEXTURE_ID::RIFLE_MAN);
}

void RifleManStanding::LoadAnimations()
{
	if (HasAnimations<RifleManStanding>::_hasBeenLoaded) {
		return;
	}
	HasAnimations<RifleManStanding>::_hasBeenLoaded = true;

	GraphicsHelper::InsertAnimation(RIFLE_MAN_ANIMATION_ID::SHOOT_NORMAL, Constants::Enemies::RifleMan::ANIMATION_STANDING_DELAY_MILLISECONDS,
		{
			{RIFLE_MAN_SPRITE_ID::SHOOT_NORMAL_01, 0},
			{RIFLE_MAN_SPRITE_ID::SHOOT_NORMAL_02, 0},
		});

	GraphicsHelper::InsertAnimation(RIFLE_MAN_ANIMATION_ID::SHOOT_UP, Constants::Enemies::RifleMan::ANIMATION_STANDING_DELAY_MILLISECONDS,
		{
			{RIFLE_MAN_SPRITE_ID::SHOOT_UP_01, 0},
			{RIFLE_MAN_SPRITE_ID::SHOOT_UP_02, 0},
		});

	GraphicsHelper::InsertAnimation(RIFLE_MAN_ANIMATION_ID::SHOOT_DOWN, Constants::Enemies::RifleMan::ANIMATION_STANDING_DELAY_MILLISECONDS,
		{
			{RIFLE_MAN_SPRITE_ID::SHOOT_DOWN, 0},
		});
}

void RifleManStanding::Fire()
{
	if (!Enemy::_target || Enemy::_target->IsDead())
	{
		return;
	}
}

void RifleManStanding::CustomFire(float x, float y, float angle, float vx, float vy, float ax, float ay, DIRECTION movingDirection)
{
	if (!Enemy::_target || Enemy::_target->IsDead())
	{
		return;
	}

	HasWeapons::Fire(x, y, angle, vx, vy, ax, ay, movingDirection);
}
