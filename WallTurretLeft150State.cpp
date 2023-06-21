#include "WallTurret.h"

WallTurretLeft150State::WallTurretLeft150State() {}

WallTurretLeft150State::~WallTurretLeft150State() {}

void WallTurretLeft150State::Exit(WallTurret& wallTurret) {}

void WallTurretLeft150State::Render(WallTurret& wallTurret) {
	wallTurret.SetAnimation(WALL_TURRET_ANIMATION_ID::LEFT_150, wallTurret.GetPosition(), wallTurret.GetMovingDirection(), wallTurret.GetAngle());
}

void WallTurretLeft150State::Enter(WallTurret& wallTurret) {}

WallTurretState* WallTurretLeft150State::Update(WallTurret& wallTurret) 
{
	if (--delayBeforeChangeState > 0)
	{
		return NULL;
	}

	FLOAT billAngle = wallTurret.CalculateBillAngle();

	if (billAngle >= -45 && billAngle < -15)
	{
		if (--shootDelay == 0)
		{
			wallTurret.Fire(0.0f, -1.0f, -1.0f * std::tan(D3DX_PI / 2 - D3DXToRadian(30)), 0.0f, 0.0f, wallTurret.GetMovingDirection());
			shootDelay = SHOOT_DELAY + STATE_CHANGE_DELAY;
		}
		return NULL;
	}

	if (billAngle < -45 || billAngle > 150)
		return new WallTurretLeft120State();

	if (billAngle >= -15 || billAngle <= 150)
		return new WallTurretDownState();

	return NULL;
}
