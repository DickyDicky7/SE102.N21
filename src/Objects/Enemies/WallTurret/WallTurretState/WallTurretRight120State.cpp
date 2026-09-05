#include <numbers>
#include "WallTurret.h"

WallTurretRight120State::WallTurretRight120State() {}

WallTurretRight120State::~WallTurretRight120State() {}

void WallTurretRight120State::Exit(WallTurret& wallTurret) {}

void WallTurretRight120State::Render(WallTurret& wallTurret) {
	wallTurret.SetAnimation(WALL_TURRET_ANIMATION_ID::RIGHT_120, wallTurret.GetPosition(), wallTurret.GetMovingDirection(), wallTurret.GetAngle());
}

void WallTurretRight120State::Enter(WallTurret& wallTurret)
{
	if (this->UpdateShooting(wallTurret))
	{
		wallTurret.Fire(0.0f, 1.0f, -1.0f * std::tan(std::numbers::pi_v<float> / 2 - D3DXToRadian(Constants::Enemies::WallTurret::AIM_ANGLE_60_DEGREES)), 0.0f, 0.0f, wallTurret.GetMovingDirection());
	}
}

WallTurretState* WallTurretRight120State::Update(WallTurret& wallTurret)
{
	if (--this->_delayBeforeChangeState > 0)
	{
		return nullptr;
	}

	float billAngle = wallTurret.CalculateBillAngle();

	if (billAngle >= Constants::Enemies::WallTurret::AIM_ANGLE_45_DEGREES && billAngle < Constants::Enemies::WallTurret::AIM_ANGLE_75_DEGREES)
		return nullptr;

	if (billAngle >= Constants::Enemies::WallTurret::AIM_ANGLE_75_DEGREES || billAngle <= -Constants::Enemies::WallTurret::AIM_ANGLE_120_DEGREES)
		return new WallTurretRight90State();

	if (billAngle < Constants::Enemies::WallTurret::AIM_ANGLE_45_DEGREES || billAngle > -Constants::Enemies::WallTurret::AIM_ANGLE_120_DEGREES)
		return new WallTurretRight150State();

	return nullptr;
}
