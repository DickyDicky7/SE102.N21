#include "RifleManStanding.h"

RifleManStandingAimUpState::RifleManStandingAimUpState()
{

}

RifleManStandingAimUpState::~RifleManStandingAimUpState()
{

}

void RifleManStandingAimUpState::Exit(RifleManStanding& rifleManStanding)
{

}

void RifleManStandingAimUpState::Enter(RifleManStanding& rifleManStanding)
{
	if (--shootDelay == 0)
	{
		shootDelay = SHOOT_DELAY;
		D3DXVECTOR3 position = rifleManStanding.GetPosition();

		FLOAT w = rifleManStanding.GetW();
		FLOAT h = rifleManStanding.GetH();

		DIRECTION movingDirection = rifleManStanding.GetMovingDirection();

		FLOAT shootingAngle = D3DXToRadian(std::abs(rifleManStanding.CalculateShootingAngle()));

		float vx = movingDirection == DIRECTION::LEFT ? -1.0f : 1.0f;
		float vy = 1.0f * std::tan(D3DX_PI / 2 - shootingAngle);

		vx = 1.0f * vx / vy;
		vy = 1.0f;

		if (movingDirection == DIRECTION::LEFT)
		{
			rifleManStanding.Fire(position.x - w * 0.5f, position.y + h, 0.0f, vx, vy, 0.0f, 0.0f, movingDirection);
			return;
		}

		rifleManStanding.Fire(position.x + w * 0.5f, position.y + h, 0.0f, vx, vy, 0.0f, 0.0f, movingDirection);
	}
	return;
}

void RifleManStandingAimUpState::Render(RifleManStanding& rifleManStanding)
{
	rifleManStanding.SetAnimation(RIFLE_MAN_ANIMATION_ID::SHOOT_UP, rifleManStanding.GetPosition(), rifleManStanding.GetMovingDirection(), rifleManStanding.GetAngle());
}

RifleManStandingState* RifleManStandingAimUpState::Update(RifleManStanding& rifleManStanding)
{
	return NULL;
}