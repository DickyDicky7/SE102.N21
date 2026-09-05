#include <numbers>
#include "WallTurret.h"

WallTurretRight30State::WallTurretRight30State() {}

WallTurretRight30State::~WallTurretRight30State() {}

void WallTurretRight30State::Exit(WallTurret& wallTurret) {}

void WallTurretRight30State::Render(WallTurret& wallTurret) {
	wallTurret.SetAnimation(WALL_TURRET_ANIMATION_ID::RIGHT_30, wallTurret.GetPosition(), wallTurret.GetMovingDirection(), wallTurret.GetAngle());
}

void WallTurretRight30State::Enter(WallTurret& wallTurret)
{
	if (this->UpdateShooting(wallTurret))
	{
		float tanValue = std::tan(std::numbers::pi_v<float> / 2 - D3DXToRadian(Constants::Enemies::WallTurret::AIM_ANGLE_30_DEGREES));
		wallTurret.Fire(0.0f, 1.0f / tanValue, 1.0f, 0.0f, 0.0f, wallTurret.GetMovingDirection());
	}
}

WallTurretState* WallTurretRight30State::Update(WallTurret& wallTurret)
{
	if (--this->_delayBeforeChangeState > 0)
	{
		return nullptr;
	}

	float billAngle = wallTurret.CalculateBillAngle();

	if (billAngle >= Constants::Enemies::WallTurret::AIM_ANGLE_135_DEGREES && billAngle < Constants::Enemies::WallTurret::AIM_ANGLE_165_DEGREES)
		return nullptr;

	if (billAngle >= Constants::Enemies::WallTurret::AIM_ANGLE_165_DEGREES || billAngle <= -Constants::Enemies::WallTurret::AIM_ANGLE_30_DEGREES)
		return new WallTurretUpState();

	if (billAngle < Constants::Enemies::WallTurret::AIM_ANGLE_135_DEGREES || billAngle > -Constants::Enemies::WallTurret::AIM_ANGLE_30_DEGREES)
		return new WallTurretRight60State();

	return nullptr;
}
