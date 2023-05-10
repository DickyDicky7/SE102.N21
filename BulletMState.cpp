#include "Bullet.h"

BulletMState::BulletMState() : BulletState()
{

}

BulletMState::~BulletMState()
{
}

void BulletMState::Exit(Bullet& bullet)
{
}

void BulletMState::Enter(Bullet& bullet)
{
}

void BulletMState::Render(Bullet& bullet)
{
	bullet.SetAnimation(BULLET_ANIMATION_ID::M, bullet.GetPosition(), bullet.GetMovingDirection(), bullet.GetAngle());
}

BulletState* BulletMState::Update(Bullet& bullet)
{
	return NULL;
}

BulletState* BulletMState::HandleInput(Bullet& bullet, Input& input)
{
	return NULL;
}
