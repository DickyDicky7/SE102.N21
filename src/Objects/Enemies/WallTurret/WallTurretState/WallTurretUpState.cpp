#include "WallTurret.h"

WallTurretUpState::WallTurretUpState() {}

WallTurretUpState::~WallTurretUpState() {}

void WallTurretUpState::Exit(WallTurret& wallTurret) {}

void WallTurretUpState::Render(WallTurret& wallTurret) {
	wallTurret.SetAnimation(WALL_TURRET_ANIMATION_ID::UP, wallTurret.GetPosition(), wallTurret.GetMovingDirection(), wallTurret.GetAngle());
}

void WallTurretUpState::Enter(WallTurret& wallTurret)
{
	if (this->UpdateShooting(wallTurret))
	{
		wallTurret.Fire(0.0f, 0.0f, 1.0f, 0.0f, 0.0f, wallTurret.GetMovingDirection());
	}
}

WallTurretState* WallTurretUpState::Update(WallTurret& wallTurret)
{
	if (--this->_delayBeforeChangeState > 0)
	{
		return nullptr;
	}

	float billAngle = wallTurret.CalculateBillAngle();

	// Up owns the sector that wraps past +/-180, so this has to be || - every
	// other turret state guards a contiguous 30-degree band and uses &&.  As
	// written with &&, "billAngle >= 165" already implied "billAngle > -165"
	// and the second test was dead; the behaviour happened to come out right
	// anyway, because angles in (-180, -165) fall past both transitions below
	// and reach the "return nullptr" at the end, which also means "stay Up".
	if (billAngle >= Constants::Enemies::WallTurret::AIM_ANGLE_165_DEGREES || billAngle < -Constants::Enemies::WallTurret::AIM_ANGLE_165_DEGREES)
		return nullptr;

	if (billAngle >= -Constants::Enemies::WallTurret::AIM_ANGLE_165_DEGREES && billAngle < 0)
		return new WallTurretLeft30State();

	if (billAngle < Constants::Enemies::WallTurret::AIM_ANGLE_165_DEGREES && billAngle > 0)
		return new WallTurretRight30State();

	return nullptr;
}
