#include "Bullet.h"

BulletBossStage1State::BulletBossStage1State() : BulletState()
{

}

BulletBossStage1State::~BulletBossStage1State()
{
}

void BulletBossStage1State::Exit(Bullet& bullet)
{
}

void BulletBossStage1State::Enter(Bullet& bullet)
{
}

void BulletBossStage1State::Render(Bullet& bullet)
{
	bullet.SetAnimation(BULLET_ANIMATION_ID::BOSS_1, bullet.GetPosition(), bullet.GetMovingDirection(), bullet.GetAngle());
}

BulletState* BulletBossStage1State::Update(Bullet& bullet)
{
	auto result = Motion::CalculateProjectileMotion({ bullet.GetX(), bullet.GetY(), +5.0f, +110.0f, time, 0.01f });
	bullet.SetVX(result.vx);
	bullet.SetVY(result.vy);
	bullet.SetX(result.x);
	bullet.SetY(result.y);
	time = result.t;
	return NULL;
}

BulletState* BulletBossStage1State::HandleInput(Bullet& bullet, Input& input)
{
	return NULL;
}

