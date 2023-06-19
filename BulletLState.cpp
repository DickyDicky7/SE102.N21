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
	Sound::getInstance()->play("shootL", false, 1);

	if (bullet.GetVX() != 0.0f && bullet.GetVY() != 0.0f)
	{
		bullet.SetY(bullet.GetY() + 3.0f);
		bullet.SetAngle(bullet.GetVX() < 0.0f ? +135.0f			   : +45.0f            );
		bullet.SetAngle(bullet.GetVY() < 0.0f ? -bullet.GetAngle() : +bullet.GetAngle());
	}
	if (bullet.GetVX() == 0.0f && bullet.GetVY() != 0.0f)
	{
		bullet.SetAngle(90.0f);
		bullet.SetX(bullet.GetX() + 2.0f);
	}
	if (bullet.GetVX() != 0.0f && bullet.GetVY() == 0.0f)
	{
		bullet.SetY(bullet.GetY() - 2.0f);
	}
}

void BulletLState::Render(Bullet& bullet)
{
	bullet.SetAnimation(BULLET_ANIMATION_ID::L, bullet.GetPosition(), bullet.GetMovingDirection(), bullet.GetAngle());
}

BulletState* BulletLState::Update(Bullet& bullet)
{
	auto resultX = Motion::CalculateUniformMotion({ bullet.GetX(), bullet.GetVX() });
	bullet.SetX(resultX.c);
	auto resultY = Motion::CalculateUniformMotion({ bullet.GetY(), bullet.GetVY() });
	bullet.SetY(resultY.c);
	return NULL;
}

BulletState* BulletLState::HandleInput(Bullet& bullet, Input& input)
{
	return NULL;
}

