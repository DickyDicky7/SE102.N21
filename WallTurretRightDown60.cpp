#include "WallTurret.h"

WallTurretRightDown60State::WallTurretRightDown60State() {}

WallTurretRightDown60State::~WallTurretRightDown60State() {}

void WallTurretRightDown60State::Exit(WallTurret& WallTurret) {}

void WallTurretRightDown60State::Render(WallTurret& WallTurret) {
	WallTurret.SetAnimation(WALL_TURRET_RIGHT_DOWN_60, WallTurret.GetPosition(), WallTurret.GetDirection());
}

void WallTurretRightDown60State::Enter(WallTurret& WallTurret) {}

WallTurretState* WallTurretRightDown60State::Update(WallTurret& WallTurret) {
	return NULL;
}
