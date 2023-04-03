#include "WallTurret.h"

WallTurretRightUp60State::WallTurretRightUp60State() {}

WallTurretRightUp60State::~WallTurretRightUp60State() {}

void WallTurretRightUp60State::Exit(WallTurret& WallTurret) {}

void WallTurretRightUp60State::Render(WallTurret& WallTurret) {
	WallTurret.SetAnimation(WALL_TURRET_RIGHT_UP_60, WallTurret.GetPosition(), WallTurret.GetDirection());
}

void WallTurretRightUp60State::Enter(WallTurret& WallTurret) {}

WallTurretState* WallTurretRightUp60State::Update(WallTurret& WallTurret) {
	return NULL;
}
