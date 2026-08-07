#include "WallTurret.h"

WallTurretLeft120State::WallTurretLeft120State() {}

WallTurretLeft120State::~WallTurretLeft120State() {}

void WallTurretLeft120State::Exit(WallTurret& wallTurret) {}

void WallTurretLeft120State::Render(WallTurret& wallTurret) {
	wallTurret.SetAnimation(WALL_TURRET_ANIMATION_ID::LEFT_120, wallTurret.GetPosition(), wallTurret.GetMovingDirection(), wallTurret.GetAngle());
}

void WallTurretLeft120State::Enter(WallTurret& wallTurret) 
{
	if (--wallTurret.shootDelay == 0)
	{
		wallTurret.Fire(0.0f, -1.0f, -1.0f * std::tan(D3DX_PI / 2 - D3DXToRadian(60)), 0.0f, 0.0f, wallTurret.GetMovingDirection());
		wallTurret.shootDelay = WALL_TURRET_SHOOT_DELAY;
	}
}

WallTurretState* WallTurretLeft120State::Update(WallTurret& wallTurret)
{
	if (--delayBeforeChangeState > 0)
	{
		return NULL;
	}

	FLOAT billAngle = wallTurret.CalculateBillAngle();

	if (billAngle >= -75 && billAngle < -45)
		return NULL;

	if (billAngle < -75 || billAngle > 120)
		return new WallTurretLeft90State();

	if (billAngle >= -45 || billAngle <= 120)
		return new WallTurretLeft150State();

	return NULL;
}
