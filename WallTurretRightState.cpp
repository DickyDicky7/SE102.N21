#include "WallTurret.h"

WallTurretRightState::WallTurretRightState() {}

WallTurretRightState::~WallTurretRightState() {}

void WallTurretRightState::Exit(WallTurret& WallTurret) {}

void WallTurretRightState::Render(WallTurret& WallTurret) {
	WallTurret.SetAnimation(WALL_TURRET_LEFT, WallTurret.GetPosition(), WallTurret.GetDirection());
}

void WallTurretRightState::Enter(WallTurret& WallTurret) {}

WallTurretState* WallTurretRightState::Update(WallTurret& WallTurret) {
	return NULL;
}
