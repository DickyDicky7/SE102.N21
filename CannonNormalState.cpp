#include "Cannon.h"

CannonNormalState::CannonNormalState()
{

}

CannonNormalState::~CannonNormalState()
{

}

void CannonNormalState::Exit(Cannon& rifleManHideOnBush)
{

}

void CannonNormalState::Enter(Cannon& rifleManHideOnBush)
{

}

void CannonNormalState::Render(Cannon& cannon)
{
	cannon.SetAnimation(CANNON_ANIMATION_ID::NORMAL, cannon.GetPosition(), cannon.GetMovingDirection(), cannon.GetAngle());
}

CannonState* CannonNormalState::Update(Cannon& cannon)
{
	float shootingAngle = cannon.CalculateShootingAngle();

	if (shootingAngle <= 75 && cannon.IsTargetInRange())
		return new CannonUp30State();

	return NULL;
}