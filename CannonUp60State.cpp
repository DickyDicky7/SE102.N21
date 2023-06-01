#include "Cannon.h"

CannonUp60State::CannonUp60State()
{

}

CannonUp60State::~CannonUp60State()
{

}

void CannonUp60State::Exit(Cannon& rifleManHideOnBush)
{

}

void CannonUp60State::Enter(Cannon& rifleManHideOnBush)
{

}

void CannonUp60State::Render(Cannon& cannon)
{
	cannon.SetAnimation(CANNON_ANIMATION_ID::UP_60, cannon.GetPosition(), cannon.GetMovingDirection(), cannon.GetAngle());
}

CannonState* CannonUp60State::Update(Cannon& cannon)
{
	float shootingAngle = cannon.CalculateShootingAngle();

	if (shootingAngle > 45 && cannon.IsTargetInRange())
		return new CannonUp30State();

	return NULL;
}