#include "WallTurret.h"

WallTurretLeft120State::WallTurretLeft120State() {}

WallTurretLeft120State::~WallTurretLeft120State() {}

void WallTurretLeft120State::Exit(WallTurret& wallTurret) {}

void WallTurretLeft120State::Render(WallTurret& wallTurret) {
	wallTurret.SetAnimation(WALL_TURRET_ANIMATIONS_ID::LEFT_120, wallTurret.GetPosition(), wallTurret.GetMovingDirection());
}

void WallTurretLeft120State::Enter(WallTurret& wallTurret) {}

WallTurretState* WallTurretLeft120State::Update(WallTurret& wallTurret) {
	return NULL;
}
