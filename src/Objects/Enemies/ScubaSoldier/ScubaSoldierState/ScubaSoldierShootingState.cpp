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
	this->_time = static_cast<float>(GetTickCount64());
}

void ScubaSoldierShootingState::Render(ScubaSoldier& scubaSoldier)
{
	scubaSoldier.SetAnimation(SCUBA_SOLDIER_ANIMATION_ID::SHOOTING, scubaSoldier.GetPosition(), scubaSoldier.GetMovingDirection(), scubaSoldier.GetAngle());
}

ScubaSoldierState* ScubaSoldierShootingState::Update(ScubaSoldier& scubaSoldier)
{
	ULONGLONG now = GetTickCount64();
	if (now - this->_time >= scubaSoldier.GetFiringRate())
	{
		scubaSoldier.Fire();
		this->_time = static_cast<float>(now);
	}
	return nullptr;
}

ScubaSoldierState* ScubaSoldierShootingState::HandleInput(ScubaSoldier& scubaSoldier, Input& input)
{
	if (input.IsKey(DIK_H))
	{
		return new ScubaSoldierHiddenState();
	}

	return nullptr;
}
