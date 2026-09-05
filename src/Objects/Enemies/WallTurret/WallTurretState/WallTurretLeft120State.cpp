#include <numbers>
#include "WallTurret.h"

WallTurretLeft120State::WallTurretLeft120State() {}

WallTurretLeft120State::~WallTurretLeft120State() {}

void WallTurretLeft120State::Exit(WallTurret& wallTurret) {}

void WallTurretLeft120State::Render(WallTurret& wallTurret) {
	wallTurret.SetAnimation(WALL_TURRET_ANIMATION_ID::LEFT_120, wallTurret.GetPosition(), wallTurret.GetMovingDirection(), wallTurret.GetAngle());
}

void WallTurretLeft120State::Enter(WallTurret& wallTurret)
{
	if (this->UpdateShooting(wallTurret))
	{
		wallTurret.Fire(0.0f, -1.0f, -1.0f * std::tan(std::numbers::pi_v<float> / 2 - D3DXToRadian(Constants::Enemies::WallTurret::AIM_ANGLE_60_DEGREES)), 0.0f, 0.0f, wallTurret.GetMovingDirection());
	}
}

WallTurretState* WallTurretLeft120State::Update(WallTurret& wallTurret)
{
	if (--this->_delayBeforeChangeState > 0)
	{
		return nullptr;
	}

	float billAngle = wallTurret.CalculateBillAngle();

	if (billAngle >= -Constants::Enemies::WallTurret::AIM_ANGLE_75_DEGREES && billAngle < -Constants::Enemies::WallTurret::AIM_ANGLE_45_DEGREES)
		return nullptr;

	if (billAngle < -Constants::Enemies::WallTurret::AIM_ANGLE_75_DEGREES || billAngle > Constants::Enemies::WallTurret::AIM_ANGLE_120_DEGREES)
		return new WallTurretLeft90State();

	if (billAngle >= -Constants::Enemies::WallTurret::AIM_ANGLE_45_DEGREES || billAngle <= Constants::Enemies::WallTurret::AIM_ANGLE_120_DEGREES)
		return new WallTurretLeft150State();

	return nullptr;
}
