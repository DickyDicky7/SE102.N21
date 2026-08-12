#include "Bullet.h"
#include "ParticleSystem.h"
#include <cmath>
#include <cstdlib>

BulletExplodeState::BulletExplodeState(DirectX::XMFLOAT4 color) : BulletState(), explodeColor(color)
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
	int count = 10 + (rand() % 5);
	for (int i = 0; i < count; ++i)
	{
		float angle = (static_cast<float>(i) / count) * 2.0f * 3.14159265f + ((rand() % 100) / 500.0f);
		float speed = 0.8f + ((rand() % 100) / 80.0f); 
		float vx = std::cos(angle) * speed;
		float vy = std::sin(angle) * speed;
		float life = 0.15f + ((rand() % 100) / 1000.0f); 
		float size = 4.0f + (rand() % 4);
		
		BulletParticleSystem::AddParticle(bullet.GetX(), bullet.GetY(), vx, vy, size, life, explodeColor);
	}
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
