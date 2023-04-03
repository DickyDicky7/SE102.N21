#include "WallTurret.h"

WallTurretLeft30State::WallTurretLeft30State() {}

WallTurretLeft30State::~WallTurretLeft30State() {}

void WallTurretLeft30State::Exit(WallTurret& wallTurret) {}

void WallTurretLeft30State::Render(WallTurret& wallTurret) {
	wallTurret.SetAnimation(WALL_TURRET_ANIMATIONS_ID::LEFT_30, wallTurret.GetPosition(), wallTurret.GetMovingDirection());
}

void WallTurretLeft30State::Enter(WallTurret& wallTurret) {}

WallTurretState* WallTurretLeft30State::Update(WallTurret& wallTurret) {
	return NULL;
}
