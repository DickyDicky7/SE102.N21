#include "RifleManStanding.h"

RifleManStandingAimUpState::RifleManStandingAimUpState()
{

}

RifleManStandingAimUpState::~RifleManStandingAimUpState()
{

}

void RifleManStandingAimUpState::Exit(RifleManStanding& rifleManHideOnBush)
{

}

void RifleManStandingAimUpState::Enter(RifleManStanding& rifleManHideOnBush)
{

}

void RifleManStandingAimUpState::Render(RifleManStanding& rifleManHideOnBush)
{
	rifleManHideOnBush.SetAnimation(RIFLE_MAN_ANIMATION_ID::SHOOT_UP, rifleManHideOnBush.GetPosition(), rifleManHideOnBush.GetMovingDirection(), rifleManHideOnBush.GetAngle());
}

RifleManStandingState* RifleManStandingAimUpState::Update(RifleManStanding& rifleManHideOnBush)
{
	return NULL;
}