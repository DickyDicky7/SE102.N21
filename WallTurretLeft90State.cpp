#include "WallTurret.h"

WallTurretLeft90State::WallTurretLeft90State(){}

WallTurretLeft90State::~WallTurretLeft90State(){}

void WallTurretLeft90State::Exit(WallTurret& wallTurret){}

void WallTurretLeft90State::Render(WallTurret& wallTurret){
	wallTurret.SetAnimation(WALL_TURRET_ANIMATIONS_ID::LEFT_90, wallTurret.GetPosition(), wallTurret.GetMovingDirection());
}

void WallTurretLeft90State::Enter(WallTurret& wallTurret){}

WallTurretState* WallTurretLeft90State::Update(WallTurret& wallTurret) {
	return NULL;
}
	