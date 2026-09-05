#include "RifleManHideOnBush.h"

RifleManHideOnBushStandingState::RifleManHideOnBushStandingState(RifleManHideOnBush& rifleManHideOnBush)
{
	this->_isShoot = false;

	const Bill* target = rifleManHideOnBush.GetEnemyTarget();
	if (target)
	{
		float dx = (rifleManHideOnBush.GetPosition().x) - (target->GetPosition().x);

		rifleManHideOnBush.SetMovingDirection(DIRECTION::RIGHT);
		if (dx > 0)
		{
			rifleManHideOnBush.SetMovingDirection(DIRECTION::LEFT);
		}
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
	if (GetTickCount64() - this->_time >= Constants::Enemies::RifleMan::STANDING_SHOOT_DELAY_MILLISECONDS && !this->_isShoot)
	{
		this->_isShoot = true;
		float w = rifleManHideOnBush.GetW();

		float offSet = rifleManHideOnBush.GetMovingDirection() == DIRECTION::LEFT ? w * -0.5f : w * 0.5f;

		float x = rifleManHideOnBush.GetX() + offSet;
		float y = rifleManHideOnBush.GetY() + Constants::Enemies::RifleMan::HIDE_ON_BUSH_BULLET_OFFSET_Y;

		float vx = rifleManHideOnBush.GetMovingDirection() == DIRECTION::LEFT ? -1.0f : 1.0f;

		rifleManHideOnBush.CustomFire(x, y, 0.0f, vx, 0.0f, 0.0f, 0.0f, rifleManHideOnBush.GetMovingDirection());
	}

	if (GetTickCount64() - this->_time > Constants::Enemies::RifleMan::STANDING_INTERVAL_MILLISECONDS)
	{
		return new RifleManHideOnBushHideState();
	}

	return nullptr;
}
