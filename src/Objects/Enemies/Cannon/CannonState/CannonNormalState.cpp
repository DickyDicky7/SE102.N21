#include "Cannon.h"

CannonNormalState::CannonNormalState()
{

}

CannonNormalState::~CannonNormalState()
{

}

void CannonNormalState::Exit(Cannon& cannon)
{

}

void CannonNormalState::Enter(Cannon& cannon)
{
	if (this->UpdateShooting(cannon))
	{
		cannon.Fire(0.0f, -1.0f, 0.0f, 0.0f, 0.0f, cannon.GetMovingDirection());
	}
}

void CannonNormalState::Render(Cannon& cannon)
{
	cannon.SetAnimation(CANNON_ANIMATION_ID::NORMAL, cannon.GetPosition(), cannon.GetMovingDirection(), cannon.GetAngle());
}

CannonState* CannonNormalState::Update(Cannon& cannon)
{
	float shootingAngle = cannon.CalculateShootingAngle();

	if (shootingAngle <= Constants::Enemies::Cannon::AIM_ANGLE_THRESHOLD_75_DEGREES && cannon.IsTargetInRange())
		return new CannonUp30State();

	return nullptr;
}
