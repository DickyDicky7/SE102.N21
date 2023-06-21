#include "WallTurret.h"

WallTurretLeft90State::WallTurretLeft90State(){}

WallTurretLeft90State::~WallTurretLeft90State(){}

void WallTurretLeft90State::Exit(WallTurret& wallTurret){}

void WallTurretLeft90State::Render(WallTurret& wallTurret){
	wallTurret.SetAnimation(WALL_TURRET_ANIMATION_ID::LEFT_90, wallTurret.GetPosition(), wallTurret.GetMovingDirection(), wallTurret.GetAngle());
}

void WallTurretLeft90State::Enter(WallTurret& wallTurret){}

WallTurretState* WallTurretLeft90State::Update(WallTurret& wallTurret)
{
	if (--delayBeforeChangeState > 0)
	{
		return NULL;
	}

	FLOAT billAngle = wallTurret.CalculateBillAngle();

	if (billAngle >= -105 && billAngle < -75)
	{
		if (--shootDelay == 0)
		{
			wallTurret.Fire(0.0f, -1.0f, 0.0f, 0.0f, 0.0f, wallTurret.GetMovingDirection());
			shootDelay = SHOOT_DELAY + STATE_CHANGE_DELAY;
		}
		return NULL;
	}

	if (billAngle < -105 || billAngle > 90)
		return new WallTurretLeft60State();

	if (billAngle >= -75 || billAngle <= 90)
		return new WallTurretLeft120State();

	return NULL;
}
	