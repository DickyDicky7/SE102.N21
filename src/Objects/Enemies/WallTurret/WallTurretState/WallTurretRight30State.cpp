#include "WallTurret.h"

WallTurretRight30State::WallTurretRight30State() {}

WallTurretRight30State::~WallTurretRight30State() {}

void WallTurretRight30State::Exit(WallTurret& wallTurret) {}

void WallTurretRight30State::Render(WallTurret& wallTurret) {
	wallTurret.SetAnimation(WALL_TURRET_ANIMATION_ID::RIGHT_30, wallTurret.GetPosition(), wallTurret.GetMovingDirection(), wallTurret.GetAngle());
}

void WallTurretRight30State::Enter(WallTurret& wallTurret) 
{
	if (--wallTurret.shootDelay == 0)
	{
		FLOAT tanValue = std::tan(D3DX_PI / 2 - D3DXToRadian(30));

		wallTurret.Fire(0.0f, 1.0f / tanValue, 1.0f, 0.0f, 0.0f, wallTurret.GetMovingDirection());
		wallTurret.shootDelay = WALL_TURRET_SHOOT_DELAY;
	}
}

WallTurretState* WallTurretRight30State::Update(WallTurret& wallTurret) 
{
	if (--delayBeforeChangeState > 0)
	{
		return NULL;
	}

	FLOAT billAngle = wallTurret.CalculateBillAngle();

	if (billAngle >= 135 && billAngle < 165)
		return NULL;

	if (billAngle >= 165 || billAngle <= -30)
		return new WallTurretUpState();

	if (billAngle < 135  || billAngle > -30)
		return new WallTurretRight60State();

	return NULL;
}
