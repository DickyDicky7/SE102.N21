#include "WallTurret.h"

WallTurretLeftUp60State::WallTurretLeftUp60State() {}

WallTurretLeftUp60State::~WallTurretLeftUp60State() {}

void WallTurretLeftUp60State::Exit(WallTurret& WallTurret) {}

void WallTurretLeftUp60State::Render(WallTurret& WallTurret) {
	WallTurret.SetAnimation(WALL_TURRET_LEFT_UP_60, WallTurret.GetPosition(), WallTurret.GetDirection());
}

void WallTurretLeftUp60State::Enter(WallTurret& WallTurret) {}

WallTurretState* WallTurretLeftUp60State::Update(WallTurret& WallTurret) {
	return NULL;
}
