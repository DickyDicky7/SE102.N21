#include "Soldier.h"

SoldierJumpState::SoldierJumpState() : SoldierState() 
{
	hasMovedLeft = 0;
	hasMovedRight = 0;
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
		soldier.SetVX(-2.0f);
		soldier.SetAX(-0.0f);
	}
	if (soldier.GetMovingDirection() == DIRECTION::RIGHT)
	{
		soldier.SetVX(+2.0f);
		soldier.SetAX(+0.0f);
	}

	soldier.SetVY(+4.00f);
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
  
	if(soldier.IsHitWall())
	soldier.SetX
	(
		soldier.GetX() + soldier.GetVX()
	);

	soldier.SetY
	(
		soldier.GetY() + soldier.GetVY() * time + soldier.GetAY() * pow(time, 2.0f) / 2.0f
	);
	soldier.SetVY
	(
		soldier.GetVY() + soldier.GetAY() * time
	);

	time += 0.05f;

	if (soldier.GetVY() <= 0 && soldier.GetY() <= 0)
	{
		soldier.SetY(0);
		return new SoldierRunState();
	}

	return NULL;
}

SoldierState* SoldierJumpState::HandleInput(Soldier& soldier, Input& input)
{
	/*if (input.Is(DIK_LEFT))
	{
		hasMovedLeft = 1;
		soldier.SetDirection(DIRECTION::LEFT);
	}
	if (input.Is(DIK_RIGHT))
	{
		hasMovedRight = 1;
		soldier.SetDirection(DIRECTION::RIGHT);
	}
	*/
	return NULL;
}