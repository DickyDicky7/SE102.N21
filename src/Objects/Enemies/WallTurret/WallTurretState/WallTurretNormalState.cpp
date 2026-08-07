#include "WallTurret.h"

WallTurretNormalState::WallTurretNormalState() {}

WallTurretNormalState::~WallTurretNormalState() {}

void WallTurretNormalState::Exit(WallTurret& wallTurret) {}

void WallTurretNormalState::Render(WallTurret& wallTurret) {
	wallTurret.SetAnimation(WALL_TURRET_ANIMATION_ID::NORMAL, wallTurret.GetPosition(), wallTurret.GetMovingDirection(), wallTurret.GetAngle());
}

void WallTurretNormalState::Enter(WallTurret& wallTurret) {}

WallTurretState* WallTurretNormalState::Update(WallTurret& wallTurret)
{
	if (wallTurret.IsTargetInRange())
	{
		return new WallTurretOpeningState();
	}

	return NULL;
}
