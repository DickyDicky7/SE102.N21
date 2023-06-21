#include "WallTurret.h"

WallTurretLeft30State::WallTurretLeft30State() {}

WallTurretLeft30State::~WallTurretLeft30State() {}

void WallTurretLeft30State::Exit(WallTurret& wallTurret) {}

void WallTurretLeft30State::Render(WallTurret& wallTurret) {
	wallTurret.SetAnimation(WALL_TURRET_ANIMATION_ID::LEFT_30, wallTurret.GetPosition(), wallTurret.GetMovingDirection(), wallTurret.GetAngle());
}

void WallTurretLeft30State::Enter(WallTurret& wallTurret) {}

WallTurretState* WallTurretLeft30State::Update(WallTurret& wallTurret)
{
	if (--delayBeforeChangeState > 0)
	{
		return NULL;
	}

	FLOAT billAngle = wallTurret.CalculateBillAngle();

	D3DXVECTOR3 position = wallTurret.GetPosition();
	FLOAT w = wallTurret.GetW();
	FLOAT h = wallTurret.GetH();

	if (billAngle >= -165 && billAngle < -135)
	{
		if (--shootDelay == 0)
		{
			wallTurret.Fire(0.0f, -1.0f, 1.0f * std::tan(D3DX_PI / 2 - D3DXToRadian(30)), 0.0f, 0.0f, wallTurret.GetMovingDirection());
			shootDelay = SHOOT_DELAY + STATE_CHANGE_DELAY;
		}
		return NULL;
	}

	if (billAngle < -165 || billAngle > 30)
		return new WallTurretUpState();

	if (billAngle >= -135 || billAngle <= 30)
		return new WallTurretLeft60State();

	return NULL;
}
