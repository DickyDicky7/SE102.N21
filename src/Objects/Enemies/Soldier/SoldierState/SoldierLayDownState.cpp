#include "Soldier.h"

SoldierLayDownState::SoldierLayDownState() : SoldierState()
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
	this->_time = static_cast<float>(GetTickCount64());
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
	if (now - this->_time > soldier.GetFiringRate())
	{
		soldier.Fire();
		this->_time = static_cast<float>(now);
	}
	return nullptr;
}

SoldierState* SoldierLayDownState::HandleInput(Soldier& soldier, Input& input)
{
	return nullptr;
}
