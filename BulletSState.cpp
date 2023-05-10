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
	return NULL;
}

BulletState* BulletSState::HandleInput(Bullet& bullet, Input& input)
{
	return NULL;
}
