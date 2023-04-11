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
	return new RifleManHideOnBushAppearState();
}