#include "WallTurret.h"

WallTurretDownState::WallTurretDownState() {}

WallTurretDownState::~WallTurretDownState() {}

void WallTurretDownState::Exit(WallTurret& wallTurret) {}

void WallTurretDownState::Render(WallTurret& wallTurret) {
	wallTurret.SetAnimation(WALL_TURRET_ANIMATION_ID::DOWN, wallTurret.GetPosition(), wallTurret.GetMovingDirection(), wallTurret.GetAngle());
}

void WallTurretDownState::Enter(WallTurret& wallTurret) 
{
	if (--wallTurret.shootDelay == 0)
	{
		wallTurret.Fire(0.0f, 0.0f, -1.0f, 0.0f, 0.0f, wallTurret.GetMovingDirection());
		wallTurret.shootDelay = WALL_TURRET_SHOOT_DELAY;
	}
}

WallTurretState* WallTurretDownState::Update(WallTurret& wallTurret) 
{
	if(--delayBeforeChangeState > 0)
	{
		return NULL;
	}

	FLOAT billAngle = wallTurret.CalculateBillAngle();

	if (billAngle >= -15 && billAngle < 15)
		return NULL;

	if (billAngle < -15)
		return new WallTurretLeft150State();

	if (billAngle >= 15)
		return new WallTurretRight150State();

	return NULL;
}
