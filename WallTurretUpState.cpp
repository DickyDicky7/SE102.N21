#include "WallTurret.h"

WallTurretUpState::WallTurretUpState() {}

WallTurretUpState::~WallTurretUpState() {}

void WallTurretUpState::Exit(WallTurret& wallTurret) {}

void WallTurretUpState::Render(WallTurret& wallTurret) {
	wallTurret.SetAnimation(WALL_TURRET_ANIMATION_ID::UP, wallTurret.GetPosition(), wallTurret.GetMovingDirection());
}

void WallTurretUpState::Enter(WallTurret& wallTurret) {}

WallTurretState* WallTurretUpState::Update(WallTurret& wallTurret) {
	return NULL;
}
