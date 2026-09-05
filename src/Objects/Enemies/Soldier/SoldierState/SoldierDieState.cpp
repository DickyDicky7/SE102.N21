#include "Soldier.h"

SoldierDieState::SoldierDieState() : SoldierState()
{
}

SoldierDieState::~SoldierDieState()
{
}

void SoldierDieState::Exit(Soldier& soldier)
{
}

void SoldierDieState::Enter(Soldier& soldier)
{
	if (soldier.GetMovingDirection() == DIRECTION::LEFT)
	{
		soldier.SetVX(+Constants::Enemies::Soldier::DIE_SPEED_X);
		soldier.SetAX(+0.0f);
	}
	if (soldier.GetMovingDirection() == DIRECTION::RIGHT)
	{
		soldier.SetVX(-Constants::Enemies::Soldier::DIE_SPEED_X);
		soldier.SetAX(-0.0f);
	}

	soldier.SetVY(+Constants::Enemies::Soldier::DIE_SPEED_Y);
	soldier.SetAY(Constants::Enemies::Soldier::DIE_ACCELERATION_Y);
}

void SoldierDieState::Render(Soldier& soldier)
{
	soldier.SetAnimation(SOLDIER_ANIMATION_ID::DIE, soldier.GetPosition(), soldier.GetMovingDirection(), soldier.GetAngle());
}

SoldierState* SoldierDieState::Update(Soldier& soldier)
{
	auto resultX = Motion::CalculateUniformMotion({ soldier.GetX(), soldier.GetVX() });
	soldier.SetX(resultX.coordinate);

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

	if (this->_time >= Constants::Enemies::Soldier::DIE_DURATION_SECONDS)
	{
		soldier.SetDead(true);
	}

	return nullptr;
}

SoldierState* SoldierDieState::HandleInput(Soldier& soldier, Input& input)
{
	return nullptr;
}
