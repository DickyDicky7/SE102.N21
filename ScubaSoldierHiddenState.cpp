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
	scubaSoldier.SetAnimation(BILL_ANIMATION_ID::NORMAL, scubaSoldier.GetPosition(), scubaSoldier.GetMovingDirection());
}

ScubaSoldierState* ScubaSoldierHiddenState::Update(ScubaSoldier& scubaSoldier)
{
	return NULL;
}

ScubaSoldierState* ScubaSoldierHiddenState::HandleInput(ScubaSoldier& scubaSoldier, Input& input)
{
	if (input.Is(DIK_H))
	{
		return new ScubaSoldierShootingState();
	}

	return NULL;
}