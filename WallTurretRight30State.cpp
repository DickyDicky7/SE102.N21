#include "WallTurret.h"

WallTurretRight30State::WallTurretRight30State() {}

WallTurretRight30State::~WallTurretRight30State() {}

void WallTurretRight30State::Exit(WallTurret& wallTurret) {}

void WallTurretRight30State::Render(WallTurret& wallTurret) {
	wallTurret.SetAnimation(WALL_TURRET_ANIMATION_ID::RIGHT_30, wallTurret.GetPosition(), wallTurret.GetMovingDirection());
}

void WallTurretRight30State::Enter(WallTurret& wallTurret) {}

WallTurretState* WallTurretRight30State::Update(WallTurret& wallTurret) {
	return NULL;
}
