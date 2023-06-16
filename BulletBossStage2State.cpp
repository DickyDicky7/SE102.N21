#include "Bullet.h"

BulletBossStage2State::BulletBossStage2State() : BulletState()
{

}

BulletBossStage2State::~BulletBossStage2State()
{
}

void BulletBossStage2State::Exit(Bullet& bullet)
{
}

void BulletBossStage2State::Enter(Bullet& bullet)
{
}

void BulletBossStage2State::Render(Bullet& bullet)
{
	bullet.SetAnimation(BULLET_ANIMATION_ID::BOSS_2, bullet.GetPosition(), bullet.GetMovingDirection(), bullet.GetAngle());
}

BulletState* BulletBossStage2State::Update(Bullet& bullet)
{
	auto resultX = Motion::CalculateUniformMotion({ bullet.GetX(), bullet.GetVX() });
	bullet.SetX(resultX.c);
	auto resultY = Motion::CalculateUniformMotion({ bullet.GetY(), bullet.GetVY() });
	bullet.SetY(resultY.c);
	return NULL;
}

BulletState* BulletBossStage2State::HandleInput(Bullet& bullet, Input& input)
{
	return NULL;
}
