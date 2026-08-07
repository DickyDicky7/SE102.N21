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
	soldier.SetVX(+1.0f);
	soldier.SetAX(+1.0f);
	soldier.SetVY(+1.0f);
	soldier.SetAY(+1.0f);
}

void SoldierLayDownState::Render(Soldier& soldier)
{
	soldier.SetAnimation(SOLDIER_ANIMATION_ID::LAY_DOWN, soldier.GetPosition(), soldier.GetMovingDirection(), soldier.GetAngle());
}

SoldierState* SoldierLayDownState::Update(Soldier& soldier)
{
	ULONGLONG now = GetTickCount64();
	if (now - time > soldier.GetFiringRate())
	{
		soldier.Fire();
		time = FLOAT(now);
	}
	return NULL;
}

SoldierState* SoldierLayDownState::HandleInput(Soldier& soldier, Input& input)
{
	//if (input.IsKey(DIK_DOWN))
	//{
	//	return NULL;
	//}
	//return new SoldierRunState();
	return NULL;
}