#include "Soldier.h"

SoldierJumpState::SoldierJumpState() : SoldierState()
{
	this->_hasMovedLeft = false;
	this->_hasMovedRight = false;
}

SoldierJumpState::~SoldierJumpState()
{
}

void SoldierJumpState::Exit(Soldier& soldier)
{
}

void SoldierJumpState::Enter(Soldier& soldier)
{
	if (soldier.GetMovingDirection() == DIRECTION::LEFT)
	{
		soldier.SetVX(-Constants::Enemies::Soldier::JUMP_SPEED_X);
		soldier.SetAX(-0.0f);
	}
	if (soldier.GetMovingDirection() == DIRECTION::RIGHT)
	{
		soldier.SetVX(+Constants::Enemies::Soldier::JUMP_SPEED_X);
		soldier.SetAX(+0.0f);
	}

	soldier.SetVY(+Constants::Enemies::Soldier::JUMP_SPEED_Y);
	soldier.SetAY(Constants::Enemies::Soldier::JUMP_ACCELERATION_Y);
}

void SoldierJumpState::Render(Soldier& soldier)
{
	soldier.SetAnimation(SOLDIER_ANIMATION_ID::JUMP, soldier.GetPosition(), soldier.GetMovingDirection(), soldier.GetAngle());
}

SoldierState* SoldierJumpState::Update(Soldier& soldier)
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

	if (soldier.GetVY() >= 0.0f)
	{
		auto resultY = Motion::CalculateUniformlyDeceleratedMotion({ soldier.GetY(), soldier.GetVY(), soldier.GetAY(), this->_time, Constants::Enemies::Soldier::MOTION_INTEGRATION_DELTA_TIME });
		this->_time = resultY.elapsedTime;
		soldier.SetY(resultY.coordinate);
		soldier.SetVY(resultY.velocity);
	}
	else
	{
		auto resultY = Motion::CalculateUniformlyAcceleratedMotion({ soldier.GetY(), soldier.GetVY(), soldier.GetAY(), this->_time, Constants::Enemies::Soldier::MOTION_INTEGRATION_DELTA_TIME });
		this->_time = resultY.elapsedTime;
		soldier.SetY(resultY.coordinate);
		soldier.SetVY(resultY.velocity);
	}

	if (soldier.GetVY() <= 0.0f && soldier.GetY() <= 0.0f)
	{
		soldier.SetY(0.0f);
		return new SoldierRunState();
	}

	return nullptr;
}

SoldierState* SoldierJumpState::HandleInput(Soldier& soldier, Input& input)
{
	return nullptr;
}
