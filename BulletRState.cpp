#include "Bullet.h"

BulletRState::BulletRState() : BulletState()
{

}

BulletRState::~BulletRState()
{
}

void BulletRState::Exit(Bullet& bullet)
{
}

void BulletRState::Enter(Bullet& bullet)
{
}

void BulletRState::Render(Bullet& bullet)
{
	bullet.SetAnimation(BULLET_ANIMATION_ID::R, bullet.GetPosition(), bullet.GetMovingDirection(), bullet.GetAngle());
}

BulletState* BulletRState::Update(Bullet& bullet)
{
	auto resultX = Motion::CalculateUniformMotion({ bullet.GetX(), bullet.GetVX() });
	bullet.SetX(resultX.c);
	auto resultY = Motion::CalculateUniformMotion({ bullet.GetY(), bullet.GetVY() });
	bullet.SetY(resultY.c);
	return NULL;
}

BulletState* BulletRState::HandleInput(Bullet& bullet, Input& input)
{
	return NULL;
}
