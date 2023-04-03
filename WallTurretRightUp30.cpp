#include "WallTurret.h"

WallTurretRightUp30State::WallTurretRightUp30State() {}

WallTurretRightUp30State::~WallTurretRightUp30State() {}

void WallTurretRightUp30State::Exit(WallTurret& WallTurret) {}

void WallTurretRightUp30State::Render(WallTurret& WallTurret) {
	WallTurret.SetAnimation(WALL_TURRET_RIGHT_UP_30, WallTurret.GetPosition(), WallTurret.GetDirection());
}

void WallTurretRightUp30State::Enter(WallTurret& WallTurret) {}

WallTurretState* WallTurretRightUp30State::Update(WallTurret& WallTurret) {
	return NULL;
}
