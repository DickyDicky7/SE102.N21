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
}

void SoldierLayDownState::Render(Soldier& soldier)
{
	soldier.SetAnimation(SOLDIER_ANIMATION_ID::LAY_DOWN, soldier.GetPosition(), soldier.GetDirection());
}

SoldierState* SoldierLayDownState::Update(Soldier& soldier)
{
	return NULL;
}

SoldierState* SoldierLayDownState::HandleInput(Soldier& soldier, Input& input)
{
	if (input.Is(DIK_DOWN))
	{
		return NULL;
	}
	return new SoldierRunState();
}