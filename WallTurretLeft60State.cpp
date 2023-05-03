#include "WallTurret.h"

WallTurretLeft60State::WallTurretLeft60State() {}

WallTurretLeft60State::~WallTurretLeft60State() {}

void WallTurretLeft60State::Exit(WallTurret& wallTurret) {}

void WallTurretLeft60State::Render(WallTurret& wallTurret) {
	wallTurret.SetAnimation(WALL_TURRET_ANIMATION_ID::LEFT_60, wallTurret.GetPosition(), wallTurret.GetMovingDirection(), wallTurret.GetAngle());
}

void WallTurretLeft60State::Enter(WallTurret& wallTurret) {}

WallTurretState* WallTurretLeft60State::Update(WallTurret& wallTurret) {
	return NULL;
}
