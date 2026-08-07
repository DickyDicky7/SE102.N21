#include "WallTurret.h"

WallTurretLeft60State::WallTurretLeft60State() {}

WallTurretLeft60State::~WallTurretLeft60State() {}

void WallTurretLeft60State::Exit(WallTurret& wallTurret) {}

void WallTurretLeft60State::Render(WallTurret& wallTurret) {
	wallTurret.SetAnimation(WALL_TURRET_ANIMATION_ID::LEFT_60, wallTurret.GetPosition(), wallTurret.GetMovingDirection(), wallTurret.GetAngle());
}

void WallTurretLeft60State::Enter(WallTurret& wallTurret) 
{
	if (--wallTurret.shootDelay == 0)
	{
		wallTurret.Fire(0.0f, -1.0f, 1.0f * std::tan(D3DX_PI / 2 - D3DXToRadian(60)), 0.0f, 0.0f, wallTurret.GetMovingDirection());
		wallTurret.shootDelay = WALL_TURRET_SHOOT_DELAY;
	}
}

WallTurretState* WallTurretLeft60State::Update(WallTurret& wallTurret) 
{
	if (--delayBeforeChangeState > 0)
	{
		return NULL;
	}

	FLOAT billAngle = wallTurret.CalculateBillAngle();

	if (billAngle >= -135 && billAngle < -105)
		return NULL;

	if (billAngle < -135 || billAngle > 60)
		return new WallTurretLeft30State();

	if (billAngle >= -105 || billAngle <= 60)
		return new WallTurretLeft90State();

	return NULL;
}
