#include "WallTurret.h"

WallTurretLeftUp30State::WallTurretLeftUp30State() {}

WallTurretLeftUp30State::~WallTurretLeftUp30State() {}

void WallTurretLeftUp30State::Exit(WallTurret& WallTurret) {}

void WallTurretLeftUp30State::Render(WallTurret& WallTurret) {
	WallTurret.SetAnimation(WALL_TURRET_LEFT_UP_30, WallTurret.GetPosition(), WallTurret.GetDirection());
}

void WallTurretLeftUp30State::Enter(WallTurret& WallTurret) {}

WallTurretState* WallTurretLeftUp30State::Update(WallTurret& WallTurret) {
	return NULL;
}
