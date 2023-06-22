#include "WallTurret.h"

WallTurretRight60State::WallTurretRight60State() {}

WallTurretRight60State::~WallTurretRight60State() {}

void WallTurretRight60State::Exit(WallTurret& wallTurret) {}

void WallTurretRight60State::Render(WallTurret& wallTurret) {
	wallTurret.SetAnimation(WALL_TURRET_ANIMATION_ID::RIGHT_60, wallTurret.GetPosition(), wallTurret.GetMovingDirection(), wallTurret.GetAngle());
}

void WallTurretRight60State::Enter(WallTurret& wallTurret) {}

WallTurretState* WallTurretRight60State::Update(WallTurret& wallTurret) 
{
	if (--delayBeforeChangeState > 0)
	{
		return NULL;
	}

	FLOAT billAngle = wallTurret.CalculateBillAngle();

	if (billAngle >= 105 && billAngle < 135)
	{
		if (--shootDelay == 0)
		{
			wallTurret.Fire(0.0f, 1.0f, 1.0f * std::tan(D3DX_PI / 2 - D3DXToRadian(60)), 0.0f, 0.0f, wallTurret.GetMovingDirection());
			shootDelay = WALL_TURRET_SHOOT_DELAY + WALL_TURRET_STATE_CHANGE_DELAY;
		}
		return NULL;
	}

	if (billAngle >= 135 || billAngle <= -60)
		return new WallTurretRight30State();

	if (billAngle < 105 || billAngle > -60)
		return new WallTurretRight90State();

	return NULL;
}
