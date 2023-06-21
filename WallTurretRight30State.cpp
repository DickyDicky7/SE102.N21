#include "WallTurret.h"

WallTurretRight30State::WallTurretRight30State() {}

WallTurretRight30State::~WallTurretRight30State() {}

void WallTurretRight30State::Exit(WallTurret& wallTurret) {}

void WallTurretRight30State::Render(WallTurret& wallTurret) {
	wallTurret.SetAnimation(WALL_TURRET_ANIMATION_ID::RIGHT_30, wallTurret.GetPosition(), wallTurret.GetMovingDirection(), wallTurret.GetAngle());
}

void WallTurretRight30State::Enter(WallTurret& wallTurret) {}

WallTurretState* WallTurretRight30State::Update(WallTurret& wallTurret) 
{
	if (--delayBeforeChangeState > 0)
	{
		return NULL;
	}

	FLOAT billAngle = wallTurret.CalculateBillAngle();

	if (billAngle >= 135 && billAngle < 165)
	{
		if (--shootDelay == 0)
		{
			wallTurret.Fire(0.0f, 1.0f, 1.0f * std::tan(D3DX_PI / 2 - D3DXToRadian(30)), 0.0f, 0.0f, wallTurret.GetMovingDirection());
			shootDelay = SHOOT_DELAY + STATE_CHANGE_DELAY;
		}
		return NULL;
	}

	if (billAngle >= 165 || billAngle <= -30)
		return new WallTurretUpState();

	if (billAngle < 135  || billAngle > -30)
		return new WallTurretRight60State();

	return NULL;
}
