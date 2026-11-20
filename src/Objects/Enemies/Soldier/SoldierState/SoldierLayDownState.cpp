#include "Soldier.h"

SoldierLayDownState::SoldierLayDownState() : SoldierState(), _lastFireTime(0)
{
}

SoldierLayDownState::~SoldierLayDownState()
{
}

void SoldierLayDownState::Exit(Soldier& soldier)
{
}

void SoldierLayDownState::Enter(Soldier& soldier)
{
	// Unlike SoldierShootState, lying down does not fire on entry: the first shot
	// waits one firing interval.  Starting from 0 instead fired on the very first
	// Update, a second shot right after the one SoldierShootState::Enter fired
	// when the soldier went straight from shooting to lying down.
	this->_lastFireTime = GetTickCount64();
	soldier.SetVX(+Constants::Enemies::Soldier::PARK_VELOCITY);
	soldier.SetAX(+Constants::Enemies::Soldier::PARK_ACCELERATION);
	soldier.SetVY(+Constants::Enemies::Soldier::PARK_VELOCITY);
	soldier.SetAY(+Constants::Enemies::Soldier::PARK_ACCELERATION);
}

void SoldierLayDownState::Render(Soldier& soldier)
{
	soldier.SetAnimation(SOLDIER_ANIMATION_ID::LAY_DOWN, soldier.GetPosition(), soldier.GetMovingDirection(), soldier.GetAngle());
}

SoldierState* SoldierLayDownState::Update(Soldier& soldier)
{
	ULONGLONG now = GetTickCount64();
	if (now - this->_lastFireTime > soldier.GetFiringRate())
	{
		soldier.Fire();
		this->_lastFireTime = now;
	}
	return nullptr;
}

SoldierState* SoldierLayDownState::HandleInput(Soldier& soldier, Input& input)
{
	return nullptr;
}

float SoldierLayDownState::GetGunMountOffsetRatio() const
{
	return Constants::Enemies::Soldier::GUN_MOUNT_OFFSET_RATIO_PRONE;
}

bool SoldierLayDownState::IsLayingDown() const
{
	return true;
}
