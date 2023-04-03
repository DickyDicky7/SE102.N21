#include "WallTurret.h"

WallTurretRightDown30State::WallTurretRightDown30State() {}

WallTurretRightDown30State::~WallTurretRightDown30State() {}

void WallTurretRightDown30State::Exit(WallTurret& WallTurret) {}

void WallTurretRightDown30State::Render(WallTurret& WallTurret) {
	WallTurret.SetAnimation(WALL_TURRET_RIGHT_DOWN_30, WallTurret.GetPosition(), WallTurret.GetDirection());
}

void WallTurretRightDown30State::Enter(WallTurret& WallTurret) {}

WallTurretState* WallTurretRightDown30State::Update(WallTurret& WallTurret) {
	return NULL;
}
