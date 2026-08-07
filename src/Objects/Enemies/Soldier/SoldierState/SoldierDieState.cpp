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
		soldier.SetVX(+1.0f);
		soldier.SetAX(+0.0f);
	}
	if (soldier.GetMovingDirection() == DIRECTION::RIGHT)
	{
		soldier.SetVX(-1.0f);
		soldier.SetAX(-0.0f);
	}

	soldier.SetVY(+2.00f);
	soldier.SetAY(-0.10f);
}

void SoldierDieState::Render(Soldier& soldier)
{
	soldier.SetAnimation(SOLDIER_ANIMATION_ID::DIE, soldier.GetPosition(), soldier.GetMovingDirection(), soldier.GetAngle());
}

SoldierState* SoldierDieState::Update(Soldier& soldier)
{
	auto resultX = Motion::CalculateUniformMotion({ soldier.GetX(), soldier.GetVX() });
	soldier.SetX(resultX.c);

	if (soldier.GetVY() >= 0.0f)
	{
		auto   resultY = Motion::CalculateUniformlyDeceleratedMotion({ soldier.GetY(), soldier.GetVY(), soldier.GetAY(), time, 0.05f });
		time = resultY.t; soldier.SetY(resultY.c); soldier.SetVY(resultY.v);
	}
	if (soldier.GetVY() <= 0.0f)
	{
		auto   resultY = Motion::CalculateUniformlyAcceleratedMotion({ soldier.GetY(), soldier.GetVY(), soldier.GetAY(), time, 0.05f });
		time = resultY.t; soldier.SetY(resultY.c); soldier.SetVY(resultY.v);
	}

	if (time >= 1.5f)
	{
		soldier.isDead = 1;
	}

	return NULL;
}

SoldierState* SoldierDieState::HandleInput(Soldier& soldier, Input& input)
{
	return NULL;
}