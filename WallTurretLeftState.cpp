#include "WallTurret.h"

WallTurretLeftState::WallTurretLeftState(){}

WallTurretLeftState::~WallTurretLeftState(){}

void WallTurretLeftState::Exit(WallTurret& WallTurret){}

void WallTurretLeftState::Render(WallTurret& WallTurret){
	WallTurret.SetAnimation(WALL_TURRET_LEFT, WallTurret.GetPosition(), WallTurret.GetDirection());
}

void WallTurretLeftState::Enter(WallTurret& WallTurret){}

WallTurretState* WallTurretLeftState::Update(WallTurret& WallTurret) {
	return NULL;
}
