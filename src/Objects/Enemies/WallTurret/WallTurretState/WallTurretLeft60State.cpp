#include <numbers>
#include "WallTurret.h"

WallTurretLeft60State::WallTurretLeft60State() {}

WallTurretLeft60State::~WallTurretLeft60State() {}

void WallTurretLeft60State::Exit(WallTurret& wallTurret) {}

void WallTurretLeft60State::Render(WallTurret& wallTurret) {
	wallTurret.SetAnimation(WALL_TURRET_ANIMATION_ID::LEFT_60, wallTurret.GetPosition(), wallTurret.GetMovingDirection(), wallTurret.GetAngle());
}

void WallTurretLeft60State::Enter(WallTurret& wallTurret)
{
	if (this->UpdateShooting(wallTurret))
	{
		wallTurret.Fire(0.0f, -1.0f, 1.0f * std::tan(std::numbers::pi_v<float> / 2 - D3DXToRadian(Constants::Enemies::WallTurret::AIM_ANGLE_60_DEGREES)), 0.0f, 0.0f, wallTurret.GetMovingDirection());
	}
}

WallTurretState* WallTurretLeft60State::Update(WallTurret& wallTurret)
{
	if (--this->_delayBeforeChangeState > 0)
	{
		return nullptr;
	}

	float billAngle = wallTurret.CalculateBillAngle();

	if (billAngle >= -Constants::Enemies::WallTurret::AIM_ANGLE_135_DEGREES && billAngle < -Constants::Enemies::WallTurret::AIM_ANGLE_105_DEGREES)
		return nullptr;

	if (billAngle < -Constants::Enemies::WallTurret::AIM_ANGLE_135_DEGREES || billAngle > Constants::Enemies::WallTurret::AIM_ANGLE_60_DEGREES)
		return new WallTurretLeft30State();

	if (billAngle >= -Constants::Enemies::WallTurret::AIM_ANGLE_105_DEGREES || billAngle <= Constants::Enemies::WallTurret::AIM_ANGLE_60_DEGREES)
		return new WallTurretLeft90State();

	return nullptr;
}
