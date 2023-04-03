#include "WallTurret.h"

WallTurretLeftDown30State::WallTurretLeftDown30State() {}

WallTurretLeftDown30State::~WallTurretLeftDown30State() {}

void WallTurretLeftDown30State::Exit(WallTurret& WallTurret) {}

void WallTurretLeftDown30State::Render(WallTurret& WallTurret) {
	WallTurret.SetAnimation(WALL_TURRET_LEFT_DOWN_30, WallTurret.GetPosition(), WallTurret.GetDirection());
}

void WallTurretLeftDown30State::Enter(WallTurret& WallTurret) {}

WallTurretState* WallTurretLeftDown30State::Update(WallTurret& WallTurret) {
	return NULL;
}
