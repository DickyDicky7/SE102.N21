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
	if (soldier.GetDirection() == LEFT)
	{
		soldier.SetVX(+2.0f);
		soldier.SetAX(+0.0f);
	}
	if (soldier.GetDirection() == RIGHT)
	{
		soldier.SetVX(-2.0f);
		soldier.SetAX(-0.0f);
	}

	soldier.SetVY(-4.00f);
	soldier.SetAY(+0.10f);
}

void SoldierJumpState::Render(Soldier& soldier)
{
	soldier.SetAnimation(BILL_JUMP, soldier.GetPosition(), soldier.GetDirection());
}

SoldierState* SoldierJumpState::Update(Soldier& soldier)
{
	if (soldier.GetDirection() == LEFT)
	{
		soldier.SetVX(-abs(soldier.GetVX()));
		soldier.SetAX(-abs(soldier.GetAX()));
	}
	if (soldier.GetDirection() == RIGHT)
	{
		soldier.SetVX(+abs(soldier.GetVX()));
		soldier.SetAX(+abs(soldier.GetAX()));
	}
	if (hasMovedLeft || hasMovedRight)
	{
		soldier.SetX
		(
			soldier.GetX() + soldier.GetVX()
		);
	}

	soldier.SetY
	(
		soldier.GetY() + soldier.GetVY() * time + soldier.GetAY() * pow(time, 2) / 2
	);
	soldier.SetVY
	(
		soldier.GetVY() + soldier.GetAY() * time
	);

	time += 0.05f;

	if (soldier.GetVY() >= 0 && soldier.GetY() >= SCREEN_HEIGHT / 2 - 50)
	{
		soldier.SetY(SCREEN_HEIGHT / 2 - 50);
		return new SoldierRunState();
	}

	return NULL;
}

SoldierState* SoldierJumpState::HandleInput(Soldier& soldier, Input& input)
{

}