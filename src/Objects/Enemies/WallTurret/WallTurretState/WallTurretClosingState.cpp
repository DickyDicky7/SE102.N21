#include "WallTurret.h"

WallTurretClosingState::WallTurretClosingState() {}

WallTurretClosingState::~WallTurretClosingState() {}

void WallTurretClosingState::Exit(WallTurret& wallTurret) {}

void WallTurretClosingState::Render(WallTurret& wallTurret) {
	wallTurret.SetAnimation(WALL_TURRET_ANIMATION_ID::CLOSING, wallTurret.GetPosition(), wallTurret.GetMovingDirection(), wallTurret.GetAngle());
}

void WallTurretClosingState::Enter(WallTurret& wallTurret) {}

WallTurretState* WallTurretClosingState::Update(WallTurret& wallTurret) {
	return NULL;
}
