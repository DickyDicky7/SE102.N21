#include "WallTurret.h"

WallTurretState::WallTurretState()
{
	this->_delayBeforeChangeState = Constants::Enemies::WallTurret::STATE_CHANGE_DELAY_FRAMES;
}

WallTurretState::~WallTurretState(){}

void WallTurretState::Enter(WallTurret& wallTurret){}

void WallTurretState::Exit(WallTurret& wallTurret){}

void WallTurretState::Render(WallTurret& wallTurret){}

WallTurretState* WallTurretState::Update(WallTurret& wallTurret) {
	return nullptr;
}

WallTurretState* WallTurretState::HandleInput(WallTurret& wallTurret, Input& input) {
	return nullptr;
}

bool WallTurretState::UpdateShooting(WallTurret& wallTurret)
{
	if (wallTurret.TickShootDelay() == 0)
	{
		wallTurret.ResetShootDelay();
		return true;
	}
	return false;
}
