#include "WallTurret.h"

WallTurretDownState::WallTurretDownState() {}

WallTurretDownState::~WallTurretDownState() {}

void WallTurretDownState::Exit(WallTurret& wallTurret) {}

void WallTurretDownState::Render(WallTurret& wallTurret) {
	wallTurret.SetAnimation(WALL_TURRET_ANIMATION_ID::DOWN, wallTurret.GetPosition(), wallTurret.GetMovingDirection(), wallTurret.GetAngle());
}

void WallTurretDownState::Enter(WallTurret& wallTurret) {}

WallTurretState* WallTurretDownState::Update(WallTurret& wallTurret) {
	return NULL;
}
