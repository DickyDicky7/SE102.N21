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
	if (--rifleManStanding.shootDelay > 0)
		return;

	if (rifleManStanding.shootTime <= 0)
	{
		rifleManStanding.shootTime = RILFE_MAN_STANDING_SHOOT_TIME;
		rifleManStanding.shootDelay = RILFE_MAN_STANDING_SHOOT_DELAY;
		rifleManStanding.shootDelayPerBullet = RILFE_MAN_STANDING_SHOOT_DELAY_PER_BULLET;
		return;
	}

	if (--rifleManStanding.shootDelayPerBullet > 0)
	{
		return;
	}

	rifleManStanding.shootTime--;
	rifleManStanding.shootDelayPerBullet = RILFE_MAN_STANDING_SHOOT_DELAY_PER_BULLET;

	D3DXVECTOR3 position = rifleManStanding.GetPosition();

	FLOAT w = rifleManStanding.GetW();
	FLOAT h = rifleManStanding.GetH();

	DIRECTION movingDirection = rifleManStanding.GetMovingDirection();

	FLOAT shootingAngle = D3DXToRadian(std::abs(rifleManStanding.CalculateShootingAngle()));

	float dy = -((rifleManStanding.GetPosition().y) - (rifleManStanding.GetEnemyTarget()->GetPosition().y));
	float vy = dy / std::abs(dy);

	float vx = movingDirection == DIRECTION::LEFT ? -1.0f : 1.0f;
	float tanValue = std::tan(D3DX_PI / 2 - shootingAngle);

	vy = vy * tanValue;

	if (tanValue > 1.0f)
	{
		vx = dy / std::abs(dy) * vx / vy;
		vy = dy / std::abs(dy);
	}

	if (movingDirection == DIRECTION::LEFT)
	{
		rifleManStanding.CustomFire(position.x - w * 0.5f, position.y + h * 0.7f, 0.0f, vx, vy, 0.0f, 0.0f, movingDirection);
		return;
	}

	rifleManStanding.CustomFire(position.x + w * 0.5f, position.y + h * 0.7f, 0.0f, vx, vy, 0.0f, 0.0f, movingDirection);

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