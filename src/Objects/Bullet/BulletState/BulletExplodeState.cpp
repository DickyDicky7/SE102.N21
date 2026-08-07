#include "Bullet.h"

BulletExplodeState::BulletExplodeState() : BulletState()
{
	time = 20.0f;
}

BulletExplodeState::~BulletExplodeState()
{
}

void BulletExplodeState::Exit(Bullet& bullet)
{
}

void BulletExplodeState::Enter(Bullet& bullet)
{
}

void BulletExplodeState::Render(Bullet& bullet)
{
	bullet.SetAnimation(BULLET_ANIMATION_ID::EXPLODE, bullet.GetPosition(), bullet.GetMovingDirection(), bullet.GetAngle());
}

BulletState* BulletExplodeState::Update(Bullet& bullet)
{
	if (--time <= 0.0f)
	{
		bullet.isDead = 1;
	}
	return NULL;
}

BulletState* BulletExplodeState::HandleInput(Bullet& bullet, Input& input)
{
	return NULL;
}
