#include "Cannon.h"

CannonUp30State::CannonUp30State()
{

}

CannonUp30State::~CannonUp30State()
{

}

void CannonUp30State::Exit(Cannon& rifleManHideOnBush)
{

}

void CannonUp30State::Enter(Cannon& rifleManHideOnBush)
{

}

void CannonUp30State::Render(Cannon& cannon)
{
	cannon.SetAnimation(CANNON_ANIMATION_ID::UP_30, cannon.GetPosition(), cannon.GetMovingDirection(), cannon.GetAngle());
}

CannonState* CannonUp30State::Update(Cannon& cannon)
{
	float shootingAngle = cannon.CalculateShootingAngle();

	bool isInRange = cannon.IsTargetInRange();

	if (shootingAngle <= 45 && isInRange)
		return new CannonUp60State();

	if (shootingAngle > 75 && isInRange)
		return new CannonNormalState();

	return NULL;
}