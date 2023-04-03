#include "WallTurret.h"

WallTurretRight60State::WallTurretRight60State() {}

WallTurretRight60State::~WallTurretRight60State() {}

void WallTurretRight60State::Exit(WallTurret& wallTurret) {}

void WallTurretRight60State::Render(WallTurret& wallTurret) {
	wallTurret.SetAnimation(WALL_TURRET_ANIMATIONS_ID::RIGHT_60, wallTurret.GetPosition(), wallTurret.GetMovingDirection());
}

void WallTurretRight60State::Enter(WallTurret& wallTurret) {}

WallTurretState* WallTurretRight60State::Update(WallTurret& wallTurret) {
	return NULL;
}
