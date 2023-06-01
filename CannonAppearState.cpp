#include "Cannon.h"

CannonAppearState::CannonAppearState()
{

}

CannonAppearState::~CannonAppearState()
{

}

void CannonAppearState::Exit(Cannon& rifleManHideOnBush)
{

}

void CannonAppearState::Enter(Cannon& rifleManHideOnBush)
{

}

void CannonAppearState::Render(Cannon& cannon)
{
	cannon.SetAnimation(CANNON_ANIMATION_ID::APPEAR, cannon.GetPosition(), cannon.GetMovingDirection(), cannon.GetAngle());
}

CannonState* CannonAppearState::Update(Cannon& cannon)
{
	std::vector<std::tuple<SPRITE_ID, TIME>>& frames = std::get<
		std::vector<std::tuple<SPRITE_ID, TIME>>>(GraphicsDatabase::animations[CANNON_ANIMATION_ID::APPEAR]);

	if (std::cmp_greater_equal(cannon.GetCurrentFrame() + 1, frames.size()))
	{
			return new CannonNormalState();
	}

	return NULL;

}