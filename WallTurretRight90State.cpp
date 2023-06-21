#include "WallTurret.h"

WallTurretRight90State::WallTurretRight90State() {}

WallTurretRight90State::~WallTurretRight90State() {}

void WallTurretRight90State::Exit(WallTurret& wallTurret) {}

void WallTurretRight90State::Render(WallTurret& wallTurret) {
	wallTurret.SetAnimation(WALL_TURRET_ANIMATION_ID::RIGHT_90, wallTurret.GetPosition(), wallTurret.GetMovingDirection(), wallTurret.GetAngle());
}

void WallTurretRight90State::Enter(WallTurret& wallTurret) {}

WallTurretState* WallTurretRight90State::Update(WallTurret& wallTurret) 
{
	if (--delayBeforeChangeState > 0)
	{
		return NULL;
	}

	FLOAT billAngle = wallTurret.CalculateBillAngle();

	if (billAngle >= 75 && billAngle < 105)
	{
		if (--shootDelay == 0)
		{
			wallTurret.Fire(0.0f, 1.0f, 0.0f, 0.0f, 0.0f, wallTurret.GetMovingDirection());
			shootDelay = SHOOT_DELAY + STATE_CHANGE_DELAY;
		}
		return NULL;
	}

	if (billAngle >= 105 || billAngle <= -90)
		return new WallTurretRight60State();

	if (billAngle < 75 || billAngle > -90)
		return new WallTurretRight120State();

	return NULL;
}
