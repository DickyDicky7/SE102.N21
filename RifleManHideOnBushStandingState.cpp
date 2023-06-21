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
		OutputDebugString(L"shoot\n");

		FLOAT w = rifleManHideOnBush.GetW();
		FLOAT h = rifleManHideOnBush.GetH();

		FLOAT x = rifleManHideOnBush.GetX() + (rifleManHideOnBush.GetMovingDirection() == DIRECTION::LEFT ? w * -0.5f : w * 0.5f);
		FLOAT y = rifleManHideOnBush.GetY() + 11.0f;

		FLOAT vx = rifleManHideOnBush.GetMovingDirection() == DIRECTION::LEFT ? -1.0f : 1.0f;

		rifleManHideOnBush.Fire(x, y, 0.0f, vx, 0.0f, 0.0f, 0.0f, rifleManHideOnBush.GetMovingDirection());
	}

	if (GetTickCount64() - this->time > 3000.0f)
	{
		return new RifleManHideOnBushHideState();
	}

	return NULL;
}