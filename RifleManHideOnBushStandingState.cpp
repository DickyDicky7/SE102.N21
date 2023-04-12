#include "RifleManHideOnBush.h"

RifleManHideOnBushStandingState::RifleManHideOnBushStandingState(RifleManHideOnBush& rifleManHideOnBush)
{
	float dx = (rifleManHideOnBush.GetPosition().x) - (rifleManHideOnBush.GetEnemyTarget()->GetPosition().x);

	rifleManHideOnBush.SetMovingDirection(DIRECTION::RIGHT);
	if (dx > 0)
	{
		rifleManHideOnBush.SetMovingDirection(DIRECTION::LEFT);
	}
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

	if (GetTickCount64() - this->time == 1500.0f)
	{
		OutputDebugString(L"\nShoot\n");
	}

	if (GetTickCount64() - this->time > 3000.0f)
	{
		return new RifleManHideOnBushHideState();
	}

	return NULL;
}