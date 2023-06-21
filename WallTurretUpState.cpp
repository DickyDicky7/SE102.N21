#include "WallTurret.h"

WallTurretUpState::WallTurretUpState() {}

WallTurretUpState::~WallTurretUpState() {}

void WallTurretUpState::Exit(WallTurret& wallTurret) {}

void WallTurretUpState::Render(WallTurret& wallTurret) {
	wallTurret.SetAnimation(WALL_TURRET_ANIMATION_ID::UP, wallTurret.GetPosition(), wallTurret.GetMovingDirection(), wallTurret.GetAngle());
}

void WallTurretUpState::Enter(WallTurret& wallTurret) {}

WallTurretState* WallTurretUpState::Update(WallTurret& wallTurret) 
{
	if (--delayBeforeChangeState > 0)
	{
		return NULL;
	}

	FLOAT billAngle = wallTurret.CalculateBillAngle();

	if (billAngle >= 165 && billAngle < -165)
	{
		if (--shootDelay == 0)
		{
			wallTurret.Fire(0.0f, 0.0f, +1.0f, 0.0f, 0.0f, wallTurret.GetMovingDirection());
			shootDelay = SHOOT_DELAY + STATE_CHANGE_DELAY;
		}
		return NULL;
	}

	if (billAngle >= -165 && billAngle < 0)
		return new WallTurretLeft30State();

	if (billAngle < 165 && billAngle > 0)
		return new WallTurretRight30State();

	return NULL;
}
