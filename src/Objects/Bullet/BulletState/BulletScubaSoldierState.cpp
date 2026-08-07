#include "Bullet.h"

BulletScubaSoldierState::BulletScubaSoldierState(FLOAT θ, FLOAT v0) : BulletState()
{
	this->θ = θ; this->v0 = v0;
}

BulletScubaSoldierState::~BulletScubaSoldierState()
{
}

void BulletScubaSoldierState::Exit(Bullet& bullet)
{
}

void BulletScubaSoldierState::Enter(Bullet& bullet)
{
}

void BulletScubaSoldierState::Render(Bullet& bullet)
{
	bullet.SetAnimation(BULLET_ANIMATION_ID::BOSS_1, bullet.GetPosition(), bullet.GetMovingDirection(), bullet.GetAngle());
}

BulletState* BulletScubaSoldierState::Update(Bullet& bullet)
{
	if (bullet.isFake 
	&&  bullet.GetVY() <= 0.0f)
		bullet.isDead = 1;

	if (bullet.GetVY() <= 0.0f)
	{
		if (bullet.GetAY() != 0.0f)
		{
			time = 0.0f;
			bullet.SetAY(0.0f);
		}
		auto result = Motion::CalculateProjectileMotion({ bullet.GetX(), bullet.GetY(), v0, 0, time, 0.01f });
		bullet.SetVX(result.vx);
		bullet.SetVY(result.vy);
		bullet.SetX(result.x);
		bullet.SetY(result.y);
		time = result.t;
	}
	else
	{
		auto   result = Motion::CalculateUniformlyDeceleratedMotion({ bullet.GetY(), bullet.GetVY(), bullet.GetAY(), time, 0.05f });
		time = result.t; bullet.SetY(result.c); bullet.SetVY(result.v);
	}
	return NULL;
}

BulletState* BulletScubaSoldierState::HandleInput(Bullet& bullet, Input& input)
{
	return NULL;
}
