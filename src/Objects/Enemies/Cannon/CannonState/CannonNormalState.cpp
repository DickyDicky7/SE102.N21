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
	if (--cannon.shootDelay > 0)
		return;

	if (cannon.shootTime <= 0)
	{
		cannon.shootTime = CANON_SHOOT_TIME;
		cannon.shootDelay = CANON_SHOOT_DELAY;
		cannon.shootDelayPerBullet = CANON_SHOOT_DELAY_PER_BULLET;
		return;
	}

	if (--cannon.shootDelayPerBullet > 0)
	{
		return;
	}

	cannon.shootTime--;
	cannon.shootDelayPerBullet = CANON_SHOOT_DELAY_PER_BULLET;

	cannon.Fire(0.0f, -1.0f, 0.0f, 0.0f, 0.0f, cannon.GetMovingDirection());
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