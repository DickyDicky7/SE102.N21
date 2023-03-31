#include "WallTurret.h"

WallTurretUpState::WallTurretUpState() {}

WallTurretUpState::~WallTurretUpState() {}

void WallTurretUpState::Exit(WallTurret& WallTurret) {}

void WallTurretUpState::Render(WallTurret& WallTurret) {
	WallTurret.SetAnimation(WALL_TURRET_LEFT, WallTurret.GetPosition(), WallTurret.GetDirection());
}

void WallTurretUpState::Enter(WallTurret& WallTurret) {}

WallTurretState* WallTurretUpState::Update(WallTurret& WallTurret) {
	return NULL;
}
