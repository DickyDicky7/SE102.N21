#include "RifleManStanding.h"

RifleManStandingNormalState::RifleManStandingNormalState()
{

}

RifleManStandingNormalState::~RifleManStandingNormalState()
{

}

void RifleManStandingNormalState::Exit(RifleManStanding& rifleManStanding)
{

}

void RifleManStandingNormalState::Enter(RifleManStanding& rifleManStanding)
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

	float dy = -((rifleManStanding.GetPosition().y) - (rifleManStanding.GetEnemyTarget()->GetPosition().y));
	float vy = dy / std::abs(dy);

	FLOAT w = rifleManStanding.GetW();
	FLOAT h = rifleManStanding.GetH();

	DIRECTION movingDirection = rifleManStanding.GetMovingDirection();

	FLOAT shootingAngle = D3DXToRadian(std::abs(rifleManStanding.CalculateShootingAngle()));

	if (movingDirection == DIRECTION::LEFT)
	{
		rifleManStanding.CustomFire(position.x - w * 0.5f, position.y + h * 0.7f, 0.0f, -1.0f, vy * std::tan(D3DX_PI / 2 - shootingAngle), 0.0f, 0.0f, movingDirection);
		return;
	}

	rifleManStanding.CustomFire(position.x + w * 0.5f, position.y + h * 0.7f, 0.0f, 1.0f, vy * std::tan(D3DX_PI / 2 - shootingAngle), 0.0f, 0.0f, movingDirection);

	return;
}

void RifleManStandingNormalState::Render(RifleManStanding& rifleManStanding)
{
	rifleManStanding.SetAnimation(RIFLE_MAN_ANIMATION_ID::SHOOT_NORMAL, rifleManStanding.GetPosition(), rifleManStanding.GetMovingDirection(), rifleManStanding.GetAngle());
}

RifleManStandingState* RifleManStandingNormalState::Update(RifleManStanding& rifleManStanding)
{
	return NULL;
}