#include "Bullet.h"

BulletFState::BulletFState(FLOAT xO, FLOAT yO, FLOAT ω) : BulletState()
{
	this->ω  = ω;
	this->xO = xO;
	this->yO = yO;

	this->r  = 20.0f;
	this->dω = 20.0f;
}

BulletFState::~BulletFState()
{
}

void BulletFState::Exit(Bullet& bullet)
{
}

void BulletFState::Enter(Bullet& bullet)
{
	if (bullet.GetVX() != 0.0f && bullet.GetVY() != 0.0f)
	{
		ω = bullet.GetVX() < 0.0f ? +135.0f : +45.0f;
		ω = bullet.GetVY() < 0.0f ? -ω	    : +ω    ;
		bullet.SetY(bullet.GetY() - 10.0f);
		bullet.SetX(bullet.GetX() + (bullet.GetVX() < 0.0f ? +5.0f : -5.0f));
	}
	if (bullet.GetVX() == 0.0f && bullet.GetVY() != 0.0f)
	{
		ω = 90.0f;
		bullet.SetY(bullet.GetY() - 10.0f);
	}
	if (bullet.GetVX() != 0.0f && bullet.GetVY() == 0.0f)
	{
		ω = bullet.GetVX() < 0.0f ? +180.0f : +00.0f;
		bullet.SetX(bullet.GetX() + (bullet.GetVX() < 0.0f ? +5.0f : -5.0f));
	}
	xO = bullet.GetX();
	yO = bullet.GetY();
}

void BulletFState::Render(Bullet& bullet)
{
	bullet.SetAnimation(BULLET_ANIMATION_ID::F, bullet.GetPosition(), bullet.GetMovingDirection(), bullet.GetAngle());
}

BulletState* BulletFState::Update(Bullet& bullet)
{
	auto resultXY = Motion::CalculateUniformCircularMotion({ r, ω, dω, xO, yO });
	bullet.SetX(resultXY.x);
	bullet.SetY(resultXY.y);
	ω = resultXY.ω;

	if (bullet.GetVX() != 0.0f)
	{
		auto resultXO = Motion::CalculateUniformMotion({ xO, bullet.GetVX() /*/ abs(bullet.GetVX()) * 1.0f*/ });
		xO = resultXO.c;
	}
	if (bullet.GetVY() != 0.0f)
	{
		auto resultYO = Motion::CalculateUniformMotion({ yO, bullet.GetVY() /*/ abs(bullet.GetVY()) * 1.0f*/ });
		yO = resultYO.c;
	}

	return NULL;
}

BulletState* BulletFState::HandleInput(Bullet& bullet, Input& input)
{
	return NULL;
}
