#include "WallTurret.h"

WallTurretRight90State::WallTurretRight90State() {}

WallTurretRight90State::~WallTurretRight90State() {}

void WallTurretRight90State::Exit(WallTurret& wallTurret) {}

void WallTurretRight90State::Render(WallTurret& wallTurret) {
	wallTurret.SetAnimation(WALL_TURRET_ANIMATION_ID::RIGHT_90, wallTurret.GetPosition(), wallTurret.GetMovingDirection());
}

void WallTurretRight90State::Enter(WallTurret& wallTurret) {}

WallTurretState* WallTurretRight90State::Update(WallTurret& wallTurret) {
	return NULL;
}
