#include "Soldier.h"

SoldierRunState::SoldierRunState() : SoldierState()
{
}

SoldierRunState::~SoldierRunState()
{
}

void SoldierRunState::Exit(Soldier&)
{
}

void SoldierRunState::Enter(Soldier& soldier)
{
	if (soldier.GetMovingDirection() == DIRECTION::LEFT)
	{
		soldier.SetVX(-Constants::Enemies::Soldier::RUN_SPEED);
		soldier.SetAX(-Constants::Enemies::Soldier::RUN_ACCELERATION);
	}
	if (soldier.GetMovingDirection() == DIRECTION::RIGHT)
	{
		soldier.SetVX(+Constants::Enemies::Soldier::RUN_SPEED);
		soldier.SetAX(+Constants::Enemies::Soldier::RUN_ACCELERATION);
	}

	soldier.SetVY(+Constants::Enemies::Soldier::PARK_VELOCITY);
	soldier.SetAY(+Constants::Enemies::Soldier::PARK_ACCELERATION);
}

void SoldierRunState::Render(Soldier& soldier)
{
	soldier.SetAnimation(SOLDIER_ANIMATION_ID::RUN, soldier.GetPosition(), soldier.GetMovingDirection(), soldier.GetAngle());
}

SoldierState* SoldierRunState::Update(Soldier& soldier)
{
	if (soldier.GetMovingDirection() == DIRECTION::LEFT)
	{
		soldier.SetVX(-std::abs(soldier.GetVX()));
		soldier.SetAX(-std::abs(soldier.GetAX()));
	}
	if (soldier.GetMovingDirection() == DIRECTION::RIGHT)
	{
		soldier.SetVX(+std::abs(soldier.GetVX()));
		soldier.SetAX(+std::abs(soldier.GetAX()));
	}

	auto result = Motion::CalculateUniformMotion({ soldier.GetX(), soldier.GetVX() });
	soldier.SetX(result.coordinate);

	return nullptr;
}

SoldierState* SoldierRunState::HandleInput(Soldier& soldier, Input& input)
{
	if (input.IsKey(DIK_SPACE))
	{
		return new SoldierJumpState();
	}
	if (input.IsKey(DIK_DOWN))
	{
		return new SoldierLayDownState();
	}
	return nullptr;
}
