#include "Cannon.h"

Cannon::Cannon() : HasWeapons(new BulletEnemyState())
{
	this->_vx = Constants::Physics::DEFAULT_INITIAL_VELOCITY_X;
	this->_vy = Constants::Physics::DEFAULT_INITIAL_VELOCITY_Y;
	this->_ax = Constants::Physics::DEFAULT_INITIAL_ACCELERATION_X;
	this->_ay = Constants::Physics::DEFAULT_INITIAL_ACCELERATION_Y;
	this->_angle = 0;
	this->_position.x = Constants::Enemies::Cannon::DEFAULT_SPAWN_X;
	this->_position.y = Constants::Enemies::Cannon::DEFAULT_SPAWN_Y;

	this->_state = nullptr;
	this->_updateState = nullptr;

	//
	this->SetDebugName(L"Cannon\n");
	//

	this->_hitCounts = Constants::Enemies::Cannon::HEALTH_POINTS;
	this->_enemyType = ENEMY_TYPE::MACHINE;

	this->_firingRate = 0;

	this->ResetBurst();
}

Cannon::~Cannon()
{
	Destroy(this->_state);
	Destroy(this->_updateState);
}

bool Cannon::IsTargetInRange() const
{
	if (!this->_target) return false;
	float dx = this->GetX() - this->_target->GetX();
	float dy = this->GetY() - this->_target->GetY();

	return (dx > 0 && dy <= 0 && this->CalculateShootingAngle() > 0);
}

const Bill* Cannon::GetEnemyTarget()
{
	return this->_target;
}

float Cannon::CalculateShootingAngle() const
{
	return this->Enemy<Bill>::CalculateShootingAngle(this);
}

void Cannon::Update()
{
	if (!this->_state)
	{
		this->_state = new CannonAppearState();
	}

	DeferState(this->_updateState, this->_state->Update(*this));

	// Firing lives in the states' Enter, so a step with no transition has to
	// re-enter the current state to keep the cannon shooting - exactly one Enter
	// per step either way, since ChangeState runs it for a transition.
	//
	// This ran once per rendered FRAME while it lived in Render, which left the
	// shootDelay countdown inside UpdateShooting paced by the display rather than
	// by the 60 Hz logic clock the rest of the cannon is tuned against.
	if (this->_updateState)
	{
		ApplyDeferredState(this->_state, this->_updateState, this);
	}
	else
	{
		this->_state->Enter(*this);
	}
}

void Cannon::Render()
{
	this->_state->Render(*this);
	this->_w = this->GetCurrentFrameW();
	this->_h = this->GetCurrentFrameH();
}

void Cannon::HandleInput(Input&)
{

}

void Cannon::LoadTextures()
{
	if (HasTextures<Cannon>::_hasBeenLoaded) {
		return;
	}
	HasTextures<Cannon>::_hasBeenLoaded = true;

	GraphicsHelper::InsertTexture(CANNON_TEXTURE_ID::CANNON, L"Resources\\Textures\\Cannon_all.bmp");
}

void Cannon::LoadSprites()
{
	if (HasSprites<Cannon>::_hasBeenLoaded) {
		return;
	}
	HasSprites<Cannon>::_hasBeenLoaded = true;

	GraphicsHelper::InsertSprite(CANNON_SPRITE_ID::HALF_APPEAR_01, 2, 2, 34, 34, DIRECTION::LEFT, CANNON_TEXTURE_ID::CANNON);
	GraphicsHelper::InsertSprite(CANNON_SPRITE_ID::HALF_APPEAR_02, 34, 2, 34, 66, DIRECTION::LEFT, CANNON_TEXTURE_ID::CANNON);
	GraphicsHelper::InsertSprite(CANNON_SPRITE_ID::HALF_APPEAR_03, 66, 2, 34, 98, DIRECTION::LEFT, CANNON_TEXTURE_ID::CANNON);

	GraphicsHelper::InsertSprite(CANNON_SPRITE_ID::THREE_FOURS_APPEAR_01, 2, 34, 66, 34, DIRECTION::LEFT, CANNON_TEXTURE_ID::CANNON);
	GraphicsHelper::InsertSprite(CANNON_SPRITE_ID::THREE_FOURS_APPEAR_02, 34, 34, 66, 66, DIRECTION::LEFT, CANNON_TEXTURE_ID::CANNON);
	GraphicsHelper::InsertSprite(CANNON_SPRITE_ID::THREE_FOURS_APPEAR_03, 66, 34, 66, 98, DIRECTION::LEFT, CANNON_TEXTURE_ID::CANNON);

	GraphicsHelper::InsertSprite(CANNON_SPRITE_ID::NORMAL_01, 2, 66, 98, 34, DIRECTION::LEFT, CANNON_TEXTURE_ID::CANNON);
	GraphicsHelper::InsertSprite(CANNON_SPRITE_ID::NORMAL_02, 34, 66, 98, 66, DIRECTION::LEFT, CANNON_TEXTURE_ID::CANNON);
	GraphicsHelper::InsertSprite(CANNON_SPRITE_ID::NORMAL_03, 66, 66, 98, 98, DIRECTION::LEFT, CANNON_TEXTURE_ID::CANNON);

	GraphicsHelper::InsertSprite(CANNON_SPRITE_ID::UP_30_01, 2, 98, 130, 34, DIRECTION::LEFT, CANNON_TEXTURE_ID::CANNON);
	GraphicsHelper::InsertSprite(CANNON_SPRITE_ID::UP_30_02, 34, 98, 130, 66, DIRECTION::LEFT, CANNON_TEXTURE_ID::CANNON);
	GraphicsHelper::InsertSprite(CANNON_SPRITE_ID::UP_30_03, 66, 98, 130, 98, DIRECTION::LEFT, CANNON_TEXTURE_ID::CANNON);

	GraphicsHelper::InsertSprite(CANNON_SPRITE_ID::UP_60_01, 2, 130, 162, 34, DIRECTION::LEFT, CANNON_TEXTURE_ID::CANNON);
	GraphicsHelper::InsertSprite(CANNON_SPRITE_ID::UP_60_02, 34, 130, 162, 66, DIRECTION::LEFT, CANNON_TEXTURE_ID::CANNON);
	GraphicsHelper::InsertSprite(CANNON_SPRITE_ID::UP_60_03, 66, 130, 162, 98, DIRECTION::LEFT, CANNON_TEXTURE_ID::CANNON);
}

void Cannon::LoadAnimations()
{
	if (HasAnimations<Cannon>::_hasBeenLoaded) {
		return;
	}
	HasAnimations<Cannon>::_hasBeenLoaded = true;

	GraphicsHelper::InsertAnimation(CANNON_ANIMATION_ID::APPEAR, Constants::Enemies::Cannon::ANIMATION_APPEAR_DELAY_MILLISECONDS,
		{
			{CANNON_SPRITE_ID::HALF_APPEAR_01, 0},
			{CANNON_SPRITE_ID::HALF_APPEAR_02, 0},
			{CANNON_SPRITE_ID::HALF_APPEAR_03, 0},
			{CANNON_SPRITE_ID::THREE_FOURS_APPEAR_01, 0},
			{CANNON_SPRITE_ID::THREE_FOURS_APPEAR_02, 0},
			{CANNON_SPRITE_ID::THREE_FOURS_APPEAR_03, 0}
		});

	GraphicsHelper::InsertAnimation(CANNON_ANIMATION_ID::NORMAL, Constants::Enemies::Cannon::ANIMATION_DEFAULT_DELAY_MILLISECONDS,
		{
			{CANNON_SPRITE_ID::NORMAL_01, 0},
			{CANNON_SPRITE_ID::NORMAL_02, 0},
			{CANNON_SPRITE_ID::NORMAL_03, 0}
		});

	GraphicsHelper::InsertAnimation(CANNON_ANIMATION_ID::UP_30, Constants::Enemies::Cannon::ANIMATION_DEFAULT_DELAY_MILLISECONDS,
		{
			{CANNON_SPRITE_ID::UP_30_01, 0},
			{CANNON_SPRITE_ID::UP_30_02, 0},
			{CANNON_SPRITE_ID::UP_30_03, 0}
		});

	GraphicsHelper::InsertAnimation(CANNON_ANIMATION_ID::UP_60, Constants::Enemies::Cannon::ANIMATION_DEFAULT_DELAY_MILLISECONDS,
		{
			{CANNON_SPRITE_ID::UP_60_01, 0},
			{CANNON_SPRITE_ID::UP_60_02, 0},
			{CANNON_SPRITE_ID::UP_60_03, 0}
		});
}

void Cannon::Fire()
{
	if (!Enemy::_target || Enemy::_target->IsDead())
	{
		return;
	}
}

void Cannon::Fire(float angle, float vx, float vy, float ax, float ay, DIRECTION direction)
{
	if (!Enemy::_target || Enemy::_target->IsDead())
	{
		return;
	}
	HasWeapons::Fire(this->_position.x, this->_position.y + this->_h / 2, angle, vx, vy, ax, ay, direction);
}
