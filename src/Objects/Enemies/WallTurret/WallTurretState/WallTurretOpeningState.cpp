#include "WallTurret.h"

WallTurretOpeningState::WallTurretOpeningState() {}

WallTurretOpeningState::~WallTurretOpeningState() {}

void WallTurretOpeningState::Exit(WallTurret& wallTurret) {}

void WallTurretOpeningState::Render(WallTurret& wallTurret) {
	wallTurret.SetAnimation(WALL_TURRET_ANIMATION_ID::OPENING, wallTurret.GetPosition(), wallTurret.GetMovingDirection(), wallTurret.GetAngle());
}

void WallTurretOpeningState::Enter(WallTurret& wallTurret) {}

WallTurretState* WallTurretOpeningState::Update(WallTurret& wallTurret) 
{
	std::vector<std::tuple<SPRITE_ID, TIME>>& frames = std::get<
		std::vector<std::tuple<SPRITE_ID, TIME>>>(GraphicsDatabase::animations[WALL_TURRET_ANIMATION_ID::OPENING]);

	if (std::cmp_greater_equal(wallTurret.GetCurrentFrame() + 1, frames.size()))
	{
		return new WallTurretLeft90State();
	}

	return NULL;
}
