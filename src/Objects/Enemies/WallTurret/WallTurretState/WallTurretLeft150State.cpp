#include <numbers>
#include "WallTurret.h"

WallTurretLeft150State::WallTurretLeft150State() {}

WallTurretLeft150State::~WallTurretLeft150State() {}

void WallTurretLeft150State::Exit(WallTurret& wallTurret) {}

void WallTurretLeft150State::Render(WallTurret& wallTurret) {
	wallTurret.SetAnimation(WALL_TURRET_ANIMATION_ID::LEFT_150, wallTurret.GetPosition(), wallTurret.GetMovingDirection(), wallTurret.GetAngle());
}

void WallTurretLeft150State::Enter(WallTurret& wallTurret)
{
	if (this->UpdateShooting(wallTurret))
	{
		float tanValue = std::tan(std::numbers::pi_v<float> / 2 - D3DXToRadian(Constants::Enemies::WallTurret::AIM_ANGLE_30_DEGREES));
		wallTurret.Fire(0.0f, -1.0f / tanValue, -1.0f, 0.0f, 0.0f, wallTurret.GetMovingDirection());
	}
}

WallTurretState* WallTurretLeft150State::Update(WallTurret& wallTurret)
{
	if (--this->_delayBeforeChangeState > 0)
	{
		return nullptr;
	}

	float billAngle = wallTurret.CalculateBillAngle();

	if (billAngle >= -Constants::Enemies::WallTurret::AIM_ANGLE_45_DEGREES && billAngle < -Constants::Enemies::WallTurret::AIM_ANGLE_15_DEGREES)
		return nullptr;

	if (billAngle < -Constants::Enemies::WallTurret::AIM_ANGLE_45_DEGREES || billAngle > Constants::Enemies::WallTurret::AIM_ANGLE_150_DEGREES)
		return new WallTurretLeft120State();

	if (billAngle >= -Constants::Enemies::WallTurret::AIM_ANGLE_15_DEGREES || billAngle <= Constants::Enemies::WallTurret::AIM_ANGLE_150_DEGREES)
		return new WallTurretDownState();

	return nullptr;
}
