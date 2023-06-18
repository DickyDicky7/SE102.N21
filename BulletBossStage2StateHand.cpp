#include "Bullet.h"

BulletBossStage2StateHand::BulletBossStage2StateHand() : BulletState()
{

}

BulletBossStage2StateHand::~BulletBossStage2StateHand()
{
}

void BulletBossStage2StateHand::Exit(Bullet& bullet)
{
}

void BulletBossStage2StateHand::Enter(Bullet& bullet)
{
}

void BulletBossStage2StateHand::Render(Bullet& bullet)
{
	bullet.SetAnimation(BULLET_ANIMATION_ID::BOSS_2, bullet.GetPosition(), bullet.GetMovingDirection(), bullet.GetAngle());
}

BulletState* BulletBossStage2StateHand::Update(Bullet& bullet)
{
	auto resultX = Motion::CalculateUniformMotion({ bullet.GetX(), bullet.GetVX() });
	bullet.SetX(resultX.c);
	auto resultY = Motion::CalculateUniformMotion({ bullet.GetY(), bullet.GetVY() });
	bullet.SetY(resultY.c);
	return NULL;
}

BulletState* BulletBossStage2StateHand::HandleInput(Bullet& bullet, Input& input)
{
	return NULL;
}
