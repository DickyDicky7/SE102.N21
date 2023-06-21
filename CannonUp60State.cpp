#include "Cannon.h"

CannonUp60State::CannonUp60State()
{

}

CannonUp60State::~CannonUp60State()
{

}

void CannonUp60State::Exit(Cannon&)
{

}

void CannonUp60State::Enter(Cannon& cannon)
{
	if (--cannon.shootDelay > 0)
		return;

	if (cannon.shootTime <= 0)
	{
		cannon.shootTime = SHOOT_TIME;
		cannon.shootDelay = SHOOT_DELAY;
		cannon.shootDelayPerBullet = SHOOT_DELAY_PER_BULLET;
		return;
	}

	if (--cannon.shootDelayPerBullet > 0)
	{
		return;
	}

	cannon.shootTime--;
	cannon.shootDelayPerBullet = SHOOT_DELAY_PER_BULLET;

	float vx = -1.0f;

	float tanValue = std::tan(D3DXToRadian(60));

	float vy = 1.0f * tanValue;

	if (tanValue > 1.0f)
	{
		vx = 1.0f * vx / vy;
		vy = 1.0f;
	}

	cannon.Fire(0.0f, vx, vy, 0.0f, 0.0f, cannon.GetMovingDirection());
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