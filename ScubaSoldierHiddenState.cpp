#include "ScubaSoldier.h"

ScubaSoldierHiddenState::ScubaSoldierHiddenState() : ScubaSoldierState()
{
}

ScubaSoldierHiddenState::~ScubaSoldierHiddenState()
{
}


void ScubaSoldierHiddenState::Exit(ScubaSoldier& scubaSoldier)
{
}

void ScubaSoldierHiddenState::Enter(ScubaSoldier& scubaSoldier)
{
}

void ScubaSoldierHiddenState::Render(ScubaSoldier& scubaSoldier)
{
	scubaSoldier.SetAnimation(SCUBA_SOLDIER_ANIMATION_ID::HIDDEN, scubaSoldier.GetPosition(), scubaSoldier.GetMovingDirection(), scubaSoldier.GetAngle());
}

ScubaSoldierState* ScubaSoldierHiddenState::Update(ScubaSoldier& scubaSoldier)
{
	return NULL;
}

ScubaSoldierState* ScubaSoldierHiddenState::HandleInput(ScubaSoldier& scubaSoldier, Input& input)
{
	if (input.IsKey(DIK_S))
	{
		return new ScubaSoldierShootingState();
	}

	return NULL;
}