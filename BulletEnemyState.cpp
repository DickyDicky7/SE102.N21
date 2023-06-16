#include "Bullet.h"

BulletEnemyState::BulletEnemyState() : BulletState()
{

}

BulletEnemyState::~BulletEnemyState()
{
}

void BulletEnemyState::Exit(Bullet& bullet)
{
}

void BulletEnemyState::Enter(Bullet& bullet)
{
}

void BulletEnemyState::Render(Bullet& bullet)
{
	bullet.SetAnimation(BULLET_ANIMATION_ID::ENEMY, bullet.GetPosition(), bullet.GetMovingDirection(), bullet.GetAngle());
}

BulletState* BulletEnemyState::Update(Bullet& bullet)
{
	auto resultX = Motion::CalculateUniformMotion({ bullet.GetX(), bullet.GetVX() });
	bullet.SetX(resultX.c);
	auto resultY = Motion::CalculateUniformMotion({ bullet.GetY(), bullet.GetVY() });
	bullet.SetY(resultY.c);
	return NULL;
}

BulletState* BulletEnemyState::HandleInput(Bullet& bullet, Input& input)
{
	return NULL;
}
