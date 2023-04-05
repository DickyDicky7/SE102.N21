#include "WallTurret.h"

WallTurretRight150State::WallTurretRight150State() {}

WallTurretRight150State::~WallTurretRight150State() {}

void WallTurretRight150State::Exit(WallTurret& wallTurret) {}

void WallTurretRight150State::Render(WallTurret& wallTurret) {
	wallTurret.SetAnimation(WALL_TURRET_ANIMATION_ID::RIGHT_150, wallTurret.GetPosition(), wallTurret.GetMovingDirection(), wallTurret.GetAngle());
}

void WallTurretRight150State::Enter(WallTurret& wallTurret) {}

WallTurretState* WallTurretRight150State::Update(WallTurret& wallTurret) {
	return NULL;
}
