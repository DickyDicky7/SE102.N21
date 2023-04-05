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
}

void SoldierDieState::Render(Soldier& soldier)
{
	soldier.SetAnimation(SOLDIER_ANIMATION_ID::DIE, soldier.GetPosition(), soldier.GetMovingDirection(), soldier.GetAngle());
}

SoldierState* SoldierDieState::Update(Soldier& soldier)
{
	return NULL;
}

SoldierState* SoldierDieState::HandleInput(Soldier& soldier, Input& input)
{
	return NULL;
}