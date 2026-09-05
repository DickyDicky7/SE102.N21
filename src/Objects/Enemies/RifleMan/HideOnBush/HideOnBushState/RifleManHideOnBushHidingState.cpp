#include "RifleManHideOnBush.h"

RifleManHideOnBushHidingState::RifleManHideOnBushHidingState()
{

}

RifleManHideOnBushHidingState::~RifleManHideOnBushHidingState()
{

}

void RifleManHideOnBushHidingState::Exit(RifleManHideOnBush& rifleManHideOnBush)
{

}

void RifleManHideOnBushHidingState::Enter(RifleManHideOnBush& rifleManHideOnBush)
{

}

void RifleManHideOnBushHidingState::Render(RifleManHideOnBush& rifleManHideOnBush)
{
	rifleManHideOnBush.SetAnimation(RIFLE_MAN_ANIMATION_ID::HIDDING, rifleManHideOnBush.GetPosition(), rifleManHideOnBush.GetMovingDirection(), rifleManHideOnBush.GetAngle());
}

RifleManHideOnBushState* RifleManHideOnBushHidingState::Update(RifleManHideOnBush& rifleManHideOnBush)
{
	if (GetTickCount64() - this->_time > Constants::Enemies::RifleMan::HIDING_INTERVAL_MILLISECONDS)
		return new RifleManHideOnBushAppearState();
	return nullptr;
}
