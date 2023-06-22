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
	Sound::getInstance()->play("shootM", false, 1);
}

void BulletMState::Render(Bullet& bullet)
{
	bullet.SetAnimation(BULLET_ANIMATION_ID::M, bullet.GetPosition(), bullet.GetMovingDirection(), bullet.GetAngle());
}

BulletState* BulletMState::Update(Bullet& bullet)
{
	auto resultX = Motion::CalculateUniformMotion({ bullet.GetX(), bullet.GetVX() });
	bullet.SetX(resultX.c);
	auto resultY = Motion::CalculateUniformMotion({ bullet.GetY(), bullet.GetVY() });
	bullet.SetY(resultY.c);
	return NULL;
}

BulletState* BulletMState::HandleInput(Bullet& bullet, Input& input)
{
	return NULL;
}
