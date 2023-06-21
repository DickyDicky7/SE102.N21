#include "RifleManStanding.h"

int RifleManStandingState::shootDelay = SHOOT_DELAY;
int RifleManStandingState::shootTime = SHOOT_TIME;
int RifleManStandingState::shootDelayPerBullet = SHOOT_DELAY_PER_BULLET;

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