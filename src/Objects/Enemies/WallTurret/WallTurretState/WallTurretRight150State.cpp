#include "WallTurret.h"

WallTurretRight150State::WallTurretRight150State() {}

WallTurretRight150State::~WallTurretRight150State() {}

void WallTurretRight150State::Exit(WallTurret& wallTurret) {}

void WallTurretRight150State::Render(WallTurret& wallTurret) {
	wallTurret.SetAnimation(WALL_TURRET_ANIMATION_ID::RIGHT_150, wallTurret.GetPosition(), wallTurret.GetMovingDirection(), wallTurret.GetAngle());
}

void WallTurretRight150State::Enter(WallTurret& wallTurret) 
{
	if (--wallTurret.shootDelay == 0)
	{
		FLOAT tanValue = std::tan(D3DX_PI / 2 - D3DXToRadian(30));

		wallTurret.Fire(0.0f, 1.0f / tanValue, -1.0f, 0.0f, 0.0f, wallTurret.GetMovingDirection());
		wallTurret.shootDelay = WALL_TURRET_SHOOT_DELAY;
	}
	return;
}

WallTurretState* WallTurretRight150State::Update(WallTurret& wallTurret)
{
	if (--delayBeforeChangeState > 0)
	{
		return NULL;
	}

	FLOAT billAngle = wallTurret.CalculateBillAngle();

	if (billAngle >= 15 && billAngle < 45)
		return NULL;

	if (billAngle >= 45 || billAngle <= -150)
		return new WallTurretRight120State();

	if (billAngle < 15 || billAngle > -150)
		return new WallTurretDownState();

	return NULL;
}
