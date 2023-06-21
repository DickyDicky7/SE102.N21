#include "RifleManStanding.h"

int RifleManStandingState::shootDelay = SHOOT_DELAY;

RifleManStandingState::RifleManStandingState()
{
	this->time = 0.0f;
}

RifleManStandingState::~RifleManStandingState()
{

}

RifleManStandingState* RifleManStandingState::HandleInput(RifleManStanding& rifleManStanding, Input& input)
{
	return NULL;
}