#include "WallTurret.h"

WallTurretState::WallTurretState()
{
	delayBeforeChangeState = WALL_TURRET_STATE_CHANGE_DELAY;
	shootDelay = WALL_TURRET_SHOOT_DELAY;
}

WallTurretState::~WallTurretState(){}

void WallTurretState::Enter(WallTurret& wallTurret){}

void WallTurretState::Exit(WallTurret& wallTurret){}

void WallTurretState::Render(WallTurret& wallTurret){}

WallTurretState* WallTurretState::Update(WallTurret& wallTurret) {
	return NULL;
}

WallTurretState* WallTurretState::HandleInput(WallTurret& wallTurret, Input& Input) {
	return NULL;
}
