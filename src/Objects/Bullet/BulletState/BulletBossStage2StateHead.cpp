#include "Bullet.h"

BulletBossStage2StateHead::BulletBossStage2StateHead() : BulletState()
{
	Sound::getInstance()->play("boss2bulletsound.wav", false, 1);
}

BulletBossStage2StateHead::~BulletBossStage2StateHead()
{
}

void BulletBossStage2StateHead::Exit(Bullet& bullet)
{
}

void BulletBossStage2StateHead::Enter(Bullet& bullet)
{
}

void BulletBossStage2StateHead::Render(Bullet& bullet)
{
	bullet.SetAnimation(BULLET_ANIMATION_ID::BOSS_2, bullet.GetPosition(), bullet.GetMovingDirection(), bullet.GetAngle());
}

BulletState* BulletBossStage2StateHead::Update(Bullet& bullet)
{
	auto resultX = Motion::CalculateUniformMotion({ bullet.GetX(), bullet.GetVX() });
	bullet.SetX(resultX.c);
	auto resultY = Motion::CalculateUniformMotion({ bullet.GetY(), bullet.GetVY() });
	bullet.SetY(resultY.c);
	return NULL;
}

BulletState* BulletBossStage2StateHead::HandleInput(Bullet& bullet, Input& input)
{
	return NULL;
}
