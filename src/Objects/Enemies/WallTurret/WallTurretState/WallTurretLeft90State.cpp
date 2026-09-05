#include "WallTurret.h"

WallTurretLeft90State::WallTurretLeft90State(){}

WallTurretLeft90State::~WallTurretLeft90State(){}

void WallTurretLeft90State::Exit(WallTurret& wallTurret){}

void WallTurretLeft90State::Render(WallTurret& wallTurret){
	wallTurret.SetAnimation(WALL_TURRET_ANIMATION_ID::LEFT_90, wallTurret.GetPosition(), wallTurret.GetMovingDirection(), wallTurret.GetAngle());
}

void WallTurretLeft90State::Enter(WallTurret& wallTurret)
{
	if (this->UpdateShooting(wallTurret))
	{
		wallTurret.Fire(0.0f, -1.0f, 0.0f, 0.0f, 0.0f, wallTurret.GetMovingDirection());
	}
}

WallTurretState* WallTurretLeft90State::Update(WallTurret& wallTurret)
{
	if (--this->_delayBeforeChangeState > 0)
	{
		return nullptr;
	}

	float billAngle = wallTurret.CalculateBillAngle();

	if (billAngle >= -Constants::Enemies::WallTurret::AIM_ANGLE_105_DEGREES && billAngle < -Constants::Enemies::WallTurret::AIM_ANGLE_75_DEGREES)
		return nullptr;

	if (billAngle < -Constants::Enemies::WallTurret::AIM_ANGLE_105_DEGREES || billAngle > Constants::Enemies::WallTurret::AIM_ANGLE_90_DEGREES)
		return new WallTurretLeft60State();

	if (billAngle >= -Constants::Enemies::WallTurret::AIM_ANGLE_75_DEGREES || billAngle <= Constants::Enemies::WallTurret::AIM_ANGLE_90_DEGREES)
		return new WallTurretLeft120State();

	return nullptr;
}

