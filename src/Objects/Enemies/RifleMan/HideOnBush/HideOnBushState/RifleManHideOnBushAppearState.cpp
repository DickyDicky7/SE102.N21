#include "RifleManHideOnBush.h"

RifleManHideOnBushAppearState::RifleManHideOnBushAppearState()
{

}

RifleManHideOnBushAppearState::~RifleManHideOnBushAppearState()
{

}

void RifleManHideOnBushAppearState::Exit(RifleManHideOnBush& rifleManHideOnBush)
{

}

void RifleManHideOnBushAppearState::Enter(RifleManHideOnBush& rifleManHideOnBush)
{

}

void RifleManHideOnBushAppearState::Render(RifleManHideOnBush& rifleManHideOnBush)
{
	rifleManHideOnBush.SetAnimation(RIFLE_MAN_ANIMATION_ID::APPEAR, rifleManHideOnBush.GetPosition(), rifleManHideOnBush.GetMovingDirection(), rifleManHideOnBush.GetAngle());
}

RifleManHideOnBushState* RifleManHideOnBushAppearState::Update(RifleManHideOnBush& rifleManHideOnBush)
{
	std::vector<std::tuple<SPRITE_ID, TIME>>& frames = std::get<
		std::vector<std::tuple<SPRITE_ID, TIME>>>(GraphicsDatabase::animations[RIFLE_MAN_ANIMATION_ID::APPEAR]);

	if (std::cmp_greater_equal(rifleManHideOnBush.GetCurrentFrame() + 1, frames.size()))
	{
		return new RifleManHideOnBushStandingState(rifleManHideOnBush);
	}
	return NULL;
}