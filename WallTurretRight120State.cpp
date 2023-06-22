#include "WallTurret.h"

WallTurretRight120State::WallTurretRight120State() {}

WallTurretRight120State::~WallTurretRight120State() {}

void WallTurretRight120State::Exit(WallTurret& wallTurret) {}

void WallTurretRight120State::Render(WallTurret& wallTurret) {
	wallTurret.SetAnimation(WALL_TURRET_ANIMATION_ID::RIGHT_120, wallTurret.GetPosition(), wallTurret.GetMovingDirection(), wallTurret.GetAngle());
}

void WallTurretRight120State::Enter(WallTurret& wallTurret) 
{
	if (--wallTurret.shootDelay == 0)
	{
		wallTurret.Fire(0.0f, 1.0f, -1.0f * std::tan(D3DX_PI / 2 - D3DXToRadian(60)), 0.0f, 0.0f, wallTurret.GetMovingDirection());
		wallTurret.shootDelay = WALL_TURRET_SHOOT_DELAY;
	}
}

WallTurretState* WallTurretRight120State::Update(WallTurret& wallTurret) 
{
	if (--delayBeforeChangeState > 0)
	{
		return NULL;
	}

	FLOAT billAngle = wallTurret.CalculateBillAngle();

	if (billAngle >= 45 && billAngle < 75)
		return NULL;

	if (billAngle >= 75 || billAngle <= -120)
		return new WallTurretRight90State();

	if (billAngle < 45 || billAngle > -120)
		return new WallTurretRight150State();

	return NULL;
}
