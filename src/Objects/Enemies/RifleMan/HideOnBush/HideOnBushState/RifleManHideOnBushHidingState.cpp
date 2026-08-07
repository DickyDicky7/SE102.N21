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
	if (GetTickCount64() - this->time > 2000.0f)
		return new RifleManHideOnBushAppearState();
	return NULL;
}