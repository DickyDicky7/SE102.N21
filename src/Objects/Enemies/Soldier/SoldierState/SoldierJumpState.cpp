#include "Soldier.h"

SoldierJumpState::SoldierJumpState() : SoldierState()
{
	hasMovedLeft = 0; hasMovedRight = 0;
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
		soldier.SetVX(-1.0f);
		soldier.SetAX(-0.0f);
	}
	if (soldier.GetMovingDirection() == DIRECTION::RIGHT)
	{
		soldier.SetVX(+1.0f);
		soldier.SetAX(+0.0f);
	}

	soldier.SetVY(+2.00f);
	soldier.SetAY(-0.10f);
}

void SoldierJumpState::Render(Soldier& soldier)
{
	soldier.SetAnimation(SOLDIER_ANIMATION_ID::JUMP, soldier.GetPosition(), soldier.GetMovingDirection(), soldier.GetAngle());
}

SoldierState* SoldierJumpState::Update(Soldier& soldier)
{
	if (soldier.GetMovingDirection() == DIRECTION::LEFT)
	{
		soldier.SetVX(-abs(soldier.GetVX()));
		soldier.SetAX(-abs(soldier.GetAX()));
	}
	if (soldier.GetMovingDirection() == DIRECTION::RIGHT)
	{
		soldier.SetVX(+abs(soldier.GetVX()));
		soldier.SetAX(+abs(soldier.GetAX()));
	}

	//if (hasMovedLeft || hasMovedRight)
	//{
	//	auto result = Motion::CalculateUniformMotion({ soldier.GetX(), soldier.GetVX() });
	//	soldier.SetX(result.c);
	//}

	auto result = Motion::CalculateUniformMotion({ soldier.GetX(), soldier.GetVX() });
	soldier.SetX(result.c);

	if (soldier.GetVY() >= 0.0f)
	{
		auto   result = Motion::CalculateUniformlyDeceleratedMotion({ soldier.GetY(), soldier.GetVY(), soldier.GetAY(), time, 0.05f });
		time = result.t; soldier.SetY(result.c); soldier.SetVY(result.v);
	}
	if (soldier.GetVY() <= 0.0f)
	{
		auto   result = Motion::CalculateUniformlyAcceleratedMotion({ soldier.GetY(), soldier.GetVY(), soldier.GetAY(), time, 0.05f });
		time = result.t; soldier.SetY(result.c); soldier.SetVY(result.v);
	}

	if (soldier.GetVY() <= 0.0f && soldier.GetY() <= 0.0f)
	{
		soldier.SetY(0.0f);
		return new SoldierRunState();
	}

	return NULL;
}

SoldierState* SoldierJumpState::HandleInput(Soldier& soldier, Input& input)
{
	//if (input.IsKey(DIK_LEFT))
	//{
	//	hasMovedLeft = 1;
	//	soldier.SetMovingDirection(DIRECTION::LEFT);
	//}
	//if (input.IsKey(DIK_RIGHT))
	//{
	//	hasMovedRight = 1;
	//	soldier.SetMovingDirection(DIRECTION::RIGHT);
	//}
	return NULL;
}