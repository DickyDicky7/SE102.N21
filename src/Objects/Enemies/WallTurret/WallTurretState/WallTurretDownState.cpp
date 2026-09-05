#include "WallTurret.h"

WallTurretDownState::WallTurretDownState() {}

WallTurretDownState::~WallTurretDownState() {}

void WallTurretDownState::Exit(WallTurret& wallTurret) {}

void WallTurretDownState::Render(WallTurret& wallTurret) {
	wallTurret.SetAnimation(WALL_TURRET_ANIMATION_ID::DOWN, wallTurret.GetPosition(), wallTurret.GetMovingDirection(), wallTurret.GetAngle());
}

void WallTurretDownState::Enter(WallTurret& wallTurret)
{
	if (this->UpdateShooting(wallTurret))
	{
		wallTurret.Fire(0.0f, 0.0f, -1.0f, 0.0f, 0.0f, wallTurret.GetMovingDirection());
	}
}

WallTurretState* WallTurretDownState::Update(WallTurret& wallTurret)
{
	if(--this->_delayBeforeChangeState > 0)
	{
		return nullptr;
	}

	float billAngle = wallTurret.CalculateBillAngle();

	if (billAngle >= -Constants::Enemies::WallTurret::AIM_ANGLE_15_DEGREES && billAngle < Constants::Enemies::WallTurret::AIM_ANGLE_15_DEGREES)
		return nullptr;

	if (billAngle < -Constants::Enemies::WallTurret::AIM_ANGLE_15_DEGREES)
		return new WallTurretLeft150State();

	if (billAngle >= Constants::Enemies::WallTurret::AIM_ANGLE_15_DEGREES)
		return new WallTurretRight150State();

	return nullptr;
}
