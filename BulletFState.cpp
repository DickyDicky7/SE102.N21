#include "Bullet.h"

BulletFState::BulletFState(FLOAT xO, FLOAT yO) : BulletState()
{
	this->r  = 10.0f;
	this->ω  = 0.0f;
	this->dω = 5.0f;
	this->xO = xO;
	this->yO = yO;
}

BulletFState::~BulletFState()
{
}

void BulletFState::Exit(Bullet& bullet)
{
}

void BulletFState::Enter(Bullet& bullet)
{
}

void BulletFState::Render(Bullet& bullet)
{
	bullet.SetAnimation(BULLET_ANIMATION_ID::F, bullet.GetPosition(), bullet.GetMovingDirection(), bullet.GetAngle());
}

BulletState* BulletFState::Update(Bullet& bullet)
{
	auto resultX = Motion::CalculateUniformMotion({ bullet.GetX(), bullet.GetVX() });
	bullet.SetX(resultX.c);
	auto resultY = Motion::CalculateUniformMotion({ bullet.GetY(), bullet.GetVY() });
	bullet.SetY(resultY.c);
	return NULL;
}

BulletState* BulletFState::HandleInput(Bullet& bullet, Input& input)
{
	return NULL;
}
