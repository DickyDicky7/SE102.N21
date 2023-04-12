#include "RifleManHideOnBush.h"

RifleManHideOnBushStandingState::RifleManHideOnBushStandingState()
{

}

RifleManHideOnBushStandingState::~RifleManHideOnBushStandingState()
{

}

void RifleManHideOnBushStandingState::Exit(RifleManHideOnBush& rifleManHideOnBush)
{

}

void RifleManHideOnBushStandingState::Enter(RifleManHideOnBush& rifleManHideOnBush)
{

}

void RifleManHideOnBushStandingState::Render(RifleManHideOnBush& rifleManHideOnBush)
{
	rifleManHideOnBush.SetAnimation(RIFLE_MAN_ANIMATION_ID::STANDING, rifleManHideOnBush.GetPosition(), rifleManHideOnBush.GetMovingDirection(), rifleManHideOnBush.GetAngle());
}

RifleManHideOnBushState* RifleManHideOnBushStandingState::Update(RifleManHideOnBush& rifleManHideOnBush)
{
	if (GetTickCount64() - this->time == 3200.0f)
	{
		OutputDebugString(L"\nShoot\n");
	}

	if (GetTickCount64() - this->time > 7800.0f)
	{
		return new RifleManHideOnBushHideState();
	}

	return NULL;
}