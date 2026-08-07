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
		soldier.SetVX(-1.50f);
		soldier.SetAX(-0.01f);
	}
	if (soldier.GetMovingDirection() == DIRECTION::RIGHT)
	{
		soldier.SetVX(+1.50f);
		soldier.SetAX(+0.01f);
	}

	soldier.SetVY(+1.0f);
	soldier.SetAY(+1.0f);
}

void SoldierRunState::Render(Soldier& soldier)
{
	soldier.SetAnimation(SOLDIER_ANIMATION_ID::RUN, soldier.GetPosition(), soldier.GetMovingDirection(), soldier.GetAngle());
}

SoldierState* SoldierRunState::Update(Soldier& soldier)
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

	auto result = Motion::CalculateUniformMotion({ soldier.GetX(), soldier.GetVX() });
	soldier.SetX(result.c);

	return NULL;
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
	return NULL;
}
