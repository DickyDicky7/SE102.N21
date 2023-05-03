#include "RifleManHideOnBush.h"

RifleManHideOnBushHideState::RifleManHideOnBushHideState()
{

}

RifleManHideOnBushHideState::~RifleManHideOnBushHideState()
{

}

void RifleManHideOnBushHideState::Exit(RifleManHideOnBush& rifleManHideOnBush)
{

}

void RifleManHideOnBushHideState::Enter(RifleManHideOnBush& rifleManHideOnBush)
{

}

void RifleManHideOnBushHideState::Render(RifleManHideOnBush& rifleManHideOnBush)
{
	rifleManHideOnBush.SetAnimation(RIFLE_MAN_ANIMATION_ID::HIDE, rifleManHideOnBush.GetPosition(), rifleManHideOnBush.GetMovingDirection(), rifleManHideOnBush.GetAngle());
}

RifleManHideOnBushState* RifleManHideOnBushHideState::Update(RifleManHideOnBush& rifleManHideOnBush)
{
	std::vector<std::tuple<SPRITE_ID, TIME>>& frames = std::get<
		std::vector<std::tuple<SPRITE_ID, TIME>>>(GraphicsDatabase::animations[RIFLE_MAN_ANIMATION_ID::HIDE]);

	if (std::cmp_greater_equal(rifleManHideOnBush.GetCurrentFrame() + 1, frames.size()))
	{
		this->time = GetTickCount64();
		return new RifleManHideOnBushHidingState();
	}
	return NULL;
}