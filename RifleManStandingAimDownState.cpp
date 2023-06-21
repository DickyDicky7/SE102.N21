#include "RifleManStanding.h"

RifleManStandingAimDownState::RifleManStandingAimDownState()
{

}

RifleManStandingAimDownState::~RifleManStandingAimDownState()
{

}

void RifleManStandingAimDownState::Exit(RifleManStanding& rifleManStanding)
{

}

void RifleManStandingAimDownState::Enter(RifleManStanding& rifleManStanding)
{
	if (--shootDelay > 0)
		return;

	if (shootTime <= 0)
	{
		shootTime = SHOOT_TIME;
		shootDelay = SHOOT_DELAY;
		shootDelayPerBullet = SHOOT_DELAY_PER_BULLET;
		return;
	}

	if (--shootDelayPerBullet > 0)
	{
		return;
	}

	shootTime--;
	shootDelayPerBullet = SHOOT_DELAY_PER_BULLET;

	D3DXVECTOR3 position = rifleManStanding.GetPosition();

	FLOAT w = rifleManStanding.GetW();
	FLOAT h = rifleManStanding.GetH();

	DIRECTION movingDirection = rifleManStanding.GetMovingDirection();

	FLOAT shootingAngle = D3DXToRadian(std::abs(rifleManStanding.CalculateShootingAngle()));

	float vx = movingDirection == DIRECTION::LEFT ? -1.0f : 1.0f;
	float vy = -1.0f * std::tan(D3DX_PI / 2 - shootingAngle);

	vx = -1.0f * vx / vy;
	vy = -1.0f;

	if (movingDirection == DIRECTION::LEFT)
	{
		rifleManStanding.CustomFire(position.x - w * 0.5f, position.y + h * 0.375f, 0.0f, vx, vy, 0.0f, 0.0f, movingDirection);
		return;
	}

	rifleManStanding.CustomFire(position.x + w * 0.5f, position.y + h * 0.375f, 0.0f, vx, vy, 0.0f, 0.0f, movingDirection);

	return;
}

void RifleManStandingAimDownState::Render(RifleManStanding& rifleManStanding)
{
	rifleManStanding.SetAnimation(RIFLE_MAN_ANIMATION_ID::SHOOT_DOWN, rifleManStanding.GetPosition(), rifleManStanding.GetMovingDirection(), rifleManStanding.GetAngle());
}

RifleManStandingState* RifleManStandingAimDownState::Update(RifleManStanding& rifleManStanding)
{
	return NULL;

}