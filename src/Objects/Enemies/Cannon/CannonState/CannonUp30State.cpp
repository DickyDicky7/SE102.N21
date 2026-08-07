#include "Cannon.h"

CannonUp30State::CannonUp30State()
{

}

CannonUp30State::~CannonUp30State()
{

}

void CannonUp30State::Exit(Cannon& cannon)
{

}

void CannonUp30State::Enter(Cannon& cannon)
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

	float vx = -1.0f;

	float tanValue = std::tan(D3DXToRadian(30));

	float vy = 1.0f * tanValue;

	if (tanValue > 1.0f)
	{
		vx = 1.0f * vx / vy;
		vy = 1.0f;
	}

	cannon.Fire(0.0f, vx, vy, 0.0f, 0.0f, cannon.GetMovingDirection());
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