#include "Soldier.h"

SoldierState::SoldierState()
{
	this->_time = 0.0f;
}

SoldierState::~SoldierState()
{
	this->_time = 0.0f;
}

float SoldierState::GetGunMountOffsetRatio() const
{
	return 0.0f;
}

bool SoldierState::IsDead() const
{
	return false;
}

bool SoldierState::IsJumping() const
{
	return false;
}

bool SoldierState::IsShooting() const
{
	return false;
}

bool SoldierState::IsLayingDown() const
{
	return false;
}
