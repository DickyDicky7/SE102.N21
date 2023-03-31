#include "WallTurret.h"

WallTurretState::WallTurretState(){}

WallTurretState::~WallTurretState(){}

void WallTurretState::Enter(WallTurret& WallTurret){}

void WallTurretState::Exit(WallTurret& WallTurret){}

void WallTurretState::Render(WallTurret& WallTurret){}

WallTurretState* WallTurretState::Update(WallTurret& WallTurret) {
	return NULL;
}

WallTurretState* WallTurretState::HandleInput(WallTurret& WallTurret, Input& Input) {
	return NULL;
}
