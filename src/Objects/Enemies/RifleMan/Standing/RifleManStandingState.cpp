#include "RifleManStanding.h"

RifleManStandingState::RifleManStandingState()
{
	this->_time = 0.0f;
}

RifleManStandingState::~RifleManStandingState()
{

}

RifleManStandingState* RifleManStandingState::HandleInput(RifleManStanding& rifleManStanding, Input& input)
{
	return nullptr;
}

bool RifleManStandingState::UpdateShooting(RifleManStanding& rifleManStanding)
{
	if (rifleManStanding.TickShootDelay() > 0)
		return false;

	if (rifleManStanding.GetShotsLeftInBurst() <= 0)
	{
		rifleManStanding.ResetBurst();
		return false;
	}

	if (rifleManStanding.TickShootDelayPerBullet() > 0)
	{
		return false;
	}

	rifleManStanding.ConsumeShotInBurst();
	rifleManStanding.ResetShootDelayPerBullet();
	return true;
}
