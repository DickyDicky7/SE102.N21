#include "RifleManStanding.h"

RifleManStandingNormalState::RifleManStandingNormalState()
{

}

RifleManStandingNormalState::~RifleManStandingNormalState()
{

}

void RifleManStandingNormalState::Exit(RifleManStanding& rifleManHideOnBush)
{

}

void RifleManStandingNormalState::Enter(RifleManStanding& rifleManHideOnBush)
{

}

void RifleManStandingNormalState::Render(RifleManStanding& rifleManHideOnBush)
{
	rifleManHideOnBush.SetAnimation(RIFLE_MAN_ANIMATION_ID::SHOOT_NORMAL, rifleManHideOnBush.GetPosition(), rifleManHideOnBush.GetMovingDirection(), rifleManHideOnBush.GetAngle());
}

RifleManStandingState* RifleManStandingNormalState::Update(RifleManStanding& rifleManHideOnBush)
{
	return NULL;

}