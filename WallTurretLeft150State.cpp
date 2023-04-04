#include "WallTurret.h"

WallTurretLeft150State::WallTurretLeft150State() {}

WallTurretLeft150State::~WallTurretLeft150State() {}

void WallTurretLeft150State::Exit(WallTurret& wallTurret) {}

void WallTurretLeft150State::Render(WallTurret& wallTurret) {
	wallTurret.SetAnimation(WALL_TURRET_ANIMATION_ID::LEFT_150, wallTurret.GetPosition(), wallTurret.GetMovingDirection());
}

void WallTurretLeft150State::Enter(WallTurret& wallTurret) {}

WallTurretState* WallTurretLeft150State::Update(WallTurret& wallTurret) {
	return NULL;
}
