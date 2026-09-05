#include "Soldier.h"

SoldierShootState::SoldierShootState() : SoldierState()
{
}

SoldierShootState::~SoldierShootState()
{
}

void SoldierShootState::Exit(Soldier& soldier)
{
}

void SoldierShootState::Enter(Soldier& soldier)
{
	soldier.Fire();
	this->_time = static_cast<float>(GetTickCount64());
	soldier.SetVX(+Constants::Enemies::Soldier::PARK_VELOCITY);
	soldier.SetAX(+Constants::Enemies::Soldier::PARK_ACCELERATION);
	soldier.SetVY(+Constants::Enemies::Soldier::PARK_VELOCITY);
	soldier.SetAY(+Constants::Enemies::Soldier::PARK_ACCELERATION);
}

void SoldierShootState::Render(Soldier& soldier)
{
	soldier.SetAnimation(SOLDIER_ANIMATION_ID::SHOOT, soldier.GetPosition(), soldier.GetMovingDirection(), soldier.GetAngle());
}

SoldierState* SoldierShootState::Update(Soldier& soldier)
{
	ULONGLONG now = GetTickCount64();
	if (now - this->_time >= soldier.GetFiringRate())
	{
		soldier.Fire();
		this->_time = static_cast<float>(now);
	}
	return nullptr;
}

SoldierState* SoldierShootState::HandleInput(Soldier& soldier, Input& input)
{
	return nullptr;
}
