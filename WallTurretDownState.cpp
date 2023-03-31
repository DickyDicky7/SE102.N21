#include "WallTurret.h"

WallTurretDownState::WallTurretDownState() {}

WallTurretDownState::~WallTurretDownState() {}

void WallTurretDownState::Exit(WallTurret& WallTurret) {}

void WallTurretDownState::Render(WallTurret& WallTurret) {
	WallTurret.SetAnimation(WALL_TURRET_LEFT, WallTurret.GetPosition(), WallTurret.GetDirection());
}

void WallTurretDownState::Enter(WallTurret& WallTurret) {}

WallTurretState* WallTurretDownState::Update(WallTurret& WallTurret) {
	return NULL;
}
