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
	soldier.SetVX(+1.0f);
	soldier.SetAX(+1.0f);
	soldier.SetVY(+1.0f);
	soldier.SetAY(+1.0f);
}

void SoldierShootState::Render(Soldier& soldier)
{
	soldier.SetAnimation(SOLDIER_ANIMATION_ID::SHOOT, soldier.GetPosition(), soldier.GetMovingDirection(), soldier.GetAngle());
}

SoldierState* SoldierShootState::Update(Soldier& soldier)
{
	ULONGLONG now = GetTickCount64();
	if (now - time > soldier.GetFiringRate())
	{
		soldier.Fire();
		time = FLOAT(now);
	}
	return NULL;
}

SoldierState* SoldierShootState::HandleInput(Soldier& soldier, Input& input)
{
	return NULL;
}