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
	scubaSoldier.Fire();
}

void ScubaSoldierShootingState::Render(ScubaSoldier& scubaSoldier)
{
	scubaSoldier.SetAnimation(SCUBA_SOLDIER_ANIMATION_ID::SHOOTING, scubaSoldier.GetPosition(), scubaSoldier.GetMovingDirection(), scubaSoldier.GetAngle());
}

ScubaSoldierState* ScubaSoldierShootingState::Update(ScubaSoldier& scubaSoldier)
{
	ULONGLONG now = GetTickCount64();
	if (now - time >= scubaSoldier.GetFiringRate())
	{
		scubaSoldier.Fire();
		time = FLOAT(now);
	}
	return NULL;
}

ScubaSoldierState* ScubaSoldierShootingState::HandleInput(ScubaSoldier& scubaSoldier, Input& input)
{
	if (input.IsKey(DIK_H))
	{
		return new ScubaSoldierHiddenState();
	}

	return NULL;
}