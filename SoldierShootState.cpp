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
}

void SoldierShootState::Render(Soldier& soldier)
{
	soldier.SetAnimation(SOLDIER_ANIMATION_ID::SHOOT, soldier.GetPosition(), soldier.GetDirection());
}

SoldierState* SoldierShootState::Update(Soldier& soldier)
{
	return NULL;
}

SoldierState* SoldierShootState::HandleInput(Soldier& soldier, Input& input)
{
	return NULL;
}