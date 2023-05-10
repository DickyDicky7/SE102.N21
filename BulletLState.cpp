#include "Bullet.h"

BulletLState::BulletLState() : BulletState()
{

}

BulletLState::~BulletLState()
{
}

void BulletLState::Exit(Bullet& bullet)
{
}

void BulletLState::Enter(Bullet& bullet)
{
}

void BulletLState::Render(Bullet& bullet)
{
	bullet.SetAnimation(BULLET_ANIMATION_ID::L, bullet.GetPosition(), bullet.GetMovingDirection(), bullet.GetAngle());
}

BulletState* BulletLState::Update(Bullet& bullet)
{
	return NULL;
}

BulletState* BulletLState::HandleInput(Bullet& bullet, Input& input)
{
	return NULL;
}

