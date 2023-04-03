#include "WallTurret.h"

WallTurretRight120State::WallTurretRight120State() {}

WallTurretRight120State::~WallTurretRight120State() {}

void WallTurretRight120State::Exit(WallTurret& wallTurret) {}

void WallTurretRight120State::Render(WallTurret& wallTurret) {
	wallTurret.SetAnimation(WALL_TURRET_ANIMATIONS_ID::RIGHT_120, wallTurret.GetPosition(), wallTurret.GetMovingDirection());
}

void WallTurretRight120State::Enter(WallTurret& wallTurret) {}

WallTurretState* WallTurretRight120State::Update(WallTurret& wallTurret) {
	return NULL;
}
