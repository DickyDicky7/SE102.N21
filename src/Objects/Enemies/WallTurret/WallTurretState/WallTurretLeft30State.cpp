#include <numbers>
#include "WallTurret.h"

WallTurretLeft30State::WallTurretLeft30State() {}

WallTurretLeft30State::~WallTurretLeft30State() {}

void WallTurretLeft30State::Exit(WallTurret& wallTurret) {}

void WallTurretLeft30State::Render(WallTurret& wallTurret) {
	wallTurret.SetAnimation(WALL_TURRET_ANIMATION_ID::LEFT_30, wallTurret.GetPosition(), wallTurret.GetMovingDirection(), wallTurret.GetAngle());
}

void WallTurretLeft30State::Enter(WallTurret& wallTurret)
{
	if (this->UpdateShooting(wallTurret))
	{
		wallTurret.Fire(0.0f, -1.0f / std::tan(std::numbers::pi_v<float> / 2 - D3DXToRadian(Constants::Enemies::WallTurret::AIM_ANGLE_30_DEGREES)), 1.0f, 0.0f, 0.0f, wallTurret.GetMovingDirection());
	}
}

WallTurretState* WallTurretLeft30State::Update(WallTurret& wallTurret)
{
	if (--this->_delayBeforeChangeState > 0)
	{
		return nullptr;
	}

	float billAngle = wallTurret.CalculateBillAngle();

	if (billAngle >= -Constants::Enemies::WallTurret::AIM_ANGLE_165_DEGREES && billAngle < -Constants::Enemies::WallTurret::AIM_ANGLE_135_DEGREES)
		return nullptr;

	if (billAngle < -Constants::Enemies::WallTurret::AIM_ANGLE_165_DEGREES || billAngle > Constants::Enemies::WallTurret::AIM_ANGLE_30_DEGREES)
		return new WallTurretUpState();

	if (billAngle >= -Constants::Enemies::WallTurret::AIM_ANGLE_135_DEGREES || billAngle <= Constants::Enemies::WallTurret::AIM_ANGLE_30_DEGREES)
		return new WallTurretLeft60State();

	return nullptr;
}
