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
	if (--shootDelay == 0)
	{
		shootDelay = SHOOT_DELAY;
		D3DXVECTOR3 position = rifleManStanding.GetPosition();

		FLOAT w = rifleManStanding.GetW();
		FLOAT h = rifleManStanding.GetH();

		DIRECTION movingDirection = rifleManStanding.GetMovingDirection();

		if (movingDirection == DIRECTION::LEFT)
		{
			rifleManStanding.Fire(position.x - w * 0.5f, position.y + h * 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, movingDirection);
			return;
		}

		rifleManStanding.Fire(position.x + w * 0.5f, position.y + h * 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, movingDirection);
	}
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