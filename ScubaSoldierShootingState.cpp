#include "ScubaSoldier.h"

ScubaSoldierShootingState::ScubaSoldierShootingState() : ScubaSoldierState()
{
}

ScubaSoldierShootingState::~ScubaSoldierShootingState()
{
}


void ScubaSoldierShootingState::Exit(ScubaSoldier& scubaSoldier)
{
}

void ScubaSoldierShootingState::Enter(ScubaSoldier& scubaSoldier)
{
}

void ScubaSoldierShootingState::Render(ScubaSoldier& scubaSoldier)
{
	scubaSoldier.SetAnimation(SCUBA_SOLDIER_ANIMATION_ID::SHOOTING, scubaSoldier.GetPosition(), scubaSoldier.GetMovingDirection());
}

ScubaSoldierState* ScubaSoldierShootingState::Update(ScubaSoldier& scubaSoldier)
{
	return NULL;
}

ScubaSoldierState* ScubaSoldierShootingState::HandleInput(ScubaSoldier& scubaSoldier, Input& input)
{
	if (input.Is(DIK_H))
	{
		return new ScubaSoldierHiddenState();
	}

	return NULL;
}