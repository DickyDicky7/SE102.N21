#include "RifleManStanding.h"

RifleManStandingAimDownState::RifleManStandingAimDownState()
{

}

RifleManStandingAimDownState::~RifleManStandingAimDownState()
{

}

void RifleManStandingAimDownState::Exit(RifleManStanding& rifleManHideOnBush)
{

}

void RifleManStandingAimDownState::Enter(RifleManStanding& rifleManHideOnBush)
{

}

void RifleManStandingAimDownState::Render(RifleManStanding& rifleManHideOnBush)
{
	rifleManHideOnBush.SetAnimation(RIFLE_MAN_ANIMATION_ID::SHOOT_DOWN, rifleManHideOnBush.GetPosition(), rifleManHideOnBush.GetMovingDirection(), rifleManHideOnBush.GetAngle());
}

RifleManStandingState* RifleManStandingAimDownState::Update(RifleManStanding& rifleManHideOnBush)
{
	return NULL;

}