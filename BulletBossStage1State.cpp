#include "Bullet.h"

BulletBossStage1State::BulletBossStage1State() : BulletState()
{
	Sound::getInstance()->play("boss2bulletsound.wav", false, 1);
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
	//i use get angle cause not know why vx is positive and negative concurrently, i use this to control bullet speed
	auto result = Motion::CalculateProjectileMotion({ bullet.GetX(), bullet.GetY(), bullet.GetAngle() <= 6.0f ? bullet.GetAngle() : 6.0f, +180.0f, time, 0.01f });
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

