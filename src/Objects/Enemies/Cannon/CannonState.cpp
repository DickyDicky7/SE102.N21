#include "Cannon.h"

CannonState::CannonState()
{
	this->_time = 0.0f;
}

CannonState::~CannonState()
{

}

CannonState* CannonState::HandleInput(Cannon& cannon, Input& input)
{
	return nullptr;
}

bool CannonState::UpdateShooting(Cannon& cannon)
{
	if (cannon.TickShootDelay() > 0)
		return false;

	if (cannon.GetShotsLeftInBurst() <= 0)
	{
		cannon.ResetBurst();
		return false;
	}

	if (cannon.TickShootDelayPerBullet() > 0)
	{
		return false;
	}

	cannon.ConsumeShotInBurst();
	cannon.ResetShootDelayPerBullet();
	return true;
}
