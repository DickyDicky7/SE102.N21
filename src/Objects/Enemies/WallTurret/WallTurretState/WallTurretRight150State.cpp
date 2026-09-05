#include <numbers>
#include "WallTurret.h"

WallTurretRight150State::WallTurretRight150State() {}

WallTurretRight150State::~WallTurretRight150State() {}

void WallTurretRight150State::Exit(WallTurret& wallTurret) {}

void WallTurretRight150State::Render(WallTurret& wallTurret) {
	wallTurret.SetAnimation(WALL_TURRET_ANIMATION_ID::RIGHT_150, wallTurret.GetPosition(), wallTurret.GetMovingDirection(), wallTurret.GetAngle());
}

void WallTurretRight150State::Enter(WallTurret& wallTurret)
{
	if (this->UpdateShooting(wallTurret))
	{
		float tanValue = std::tan(std::numbers::pi_v<float> / 2 - D3DXToRadian(Constants::Enemies::WallTurret::AIM_ANGLE_30_DEGREES));
		wallTurret.Fire(0.0f, 1.0f / tanValue, -1.0f, 0.0f, 0.0f, wallTurret.GetMovingDirection());
	}
}

WallTurretState* WallTurretRight150State::Update(WallTurret& wallTurret)
{
	if (--this->_delayBeforeChangeState > 0)
	{
		return nullptr;
	}

	float billAngle = wallTurret.CalculateBillAngle();

	if (billAngle >= Constants::Enemies::WallTurret::AIM_ANGLE_15_DEGREES && billAngle < Constants::Enemies::WallTurret::AIM_ANGLE_45_DEGREES)
		return nullptr;

	if (billAngle >= Constants::Enemies::WallTurret::AIM_ANGLE_45_DEGREES || billAngle <= -Constants::Enemies::WallTurret::AIM_ANGLE_150_DEGREES)
		return new WallTurretRight120State();

	if (billAngle < Constants::Enemies::WallTurret::AIM_ANGLE_15_DEGREES || billAngle > -Constants::Enemies::WallTurret::AIM_ANGLE_150_DEGREES)
		return new WallTurretDownState();

	return nullptr;
}
