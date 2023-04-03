#include "WallTurret.h"

WallTurretLeftDown60State::WallTurretLeftDown60State() {}

WallTurretLeftDown60State::~WallTurretLeftDown60State() {}

void WallTurretLeftDown60State::Exit(WallTurret& WallTurret) {}

void WallTurretLeftDown60State::Render(WallTurret& WallTurret) {
	WallTurret.SetAnimation(WALL_TURRET_LEFT_DOWN_60, WallTurret.GetPosition(), WallTurret.GetDirection());
}

void WallTurretLeftDown60State::Enter(WallTurret& WallTurret) {}

WallTurretState* WallTurretLeftDown60State::Update(WallTurret& WallTurret) {
	return NULL;
}
