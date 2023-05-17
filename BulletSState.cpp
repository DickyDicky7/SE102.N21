#include "Bullet.h"

BulletSState::BulletSState() : BulletState()
{

}

BulletSState::~BulletSState()
{
}

void BulletSState::Exit(Bullet& bullet)
{
}

void BulletSState::Enter(Bullet& bullet)
{
}

void BulletSState::Render(Bullet& bullet)
{
	bullet.SetAnimation(BULLET_ANIMATION_ID::S, bullet.GetPosition(), bullet.GetMovingDirection(), bullet.GetAngle());
}

BulletState* BulletSState::Update(Bullet& bullet)
{
	auto resultX = Motion::CalculateUniformMotion({ bullet.GetX(), bullet.GetVX() });
	bullet.SetX(resultX.c);
	auto resultY = Motion::CalculateUniformMotion({ bullet.GetY(), bullet.GetVY() });
	bullet.SetY(resultY.c);
	return NULL;
}

BulletState* BulletSState::HandleInput(Bullet& bullet, Input& input)
{
	return NULL;
}

const std::vector<FLOAT> BulletSState::spreadDegrees{ +10.0f, -10.0f, +20.0f, -20.0f };
