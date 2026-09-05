#include "Bullet.h"
#include "ParticleSystem.h"
#include <cmath>
#include <cstdlib>

BulletExplodeState::BulletExplodeState(DirectX::XMFLOAT4 color) : BulletState(), _explodeColor(color)
{
	this->_time = Constants::Weapons::BULLET_EXPLODE_DURATION_FRAMES;
}

BulletExplodeState::~BulletExplodeState()
{
}

void BulletExplodeState::Exit(Bullet& bullet)
{
}

void BulletExplodeState::Enter(Bullet& bullet)
{
	int count = Constants::Particles::EXPLODE_COUNT_BASE + (rand() % Constants::Particles::EXPLODE_COUNT_JITTER_MODULO);
	for (int i = 0; i < count; ++i)
	{
		float angle = (static_cast<float>(i) / count) * Constants::Physics::TURN_RADIANS + ((rand() % Constants::Particles::RANDOM_PERCENT_MODULO) / Constants::Particles::EXPLODE_ANGLE_JITTER_DIVISOR);
		float speed = Constants::Particles::EXPLODE_SPEED_BASE + ((rand() % Constants::Particles::RANDOM_PERCENT_MODULO) / Constants::Particles::EXPLODE_SPEED_JITTER_DIVISOR);
		float vx = std::cos(angle) * speed;
		float vy = std::sin(angle) * speed;
		float life = Constants::Particles::EXPLODE_LIFE_BASE + ((rand() % Constants::Particles::RANDOM_PERCENT_MODULO) / Constants::Particles::EXPLODE_LIFE_JITTER_DIVISOR);
		float size = Constants::Particles::EXPLODE_SIZE_BASE + (rand() % Constants::Particles::EXPLODE_SIZE_JITTER_MODULO);

		BulletParticleSystem::AddParticle(bullet.GetX(), bullet.GetY(), vx, vy, size, life, this->_explodeColor);
	}
}

void BulletExplodeState::Render(Bullet& bullet)
{
	bullet.SetAnimation(BULLET_ANIMATION_ID::EXPLODE, bullet.GetPosition(), bullet.GetMovingDirection(), bullet.GetAngle());
}

BulletState* BulletExplodeState::Update(Bullet& bullet)
{
	if (--this->_time <= 0.0f)
	{
		bullet.SetDead(true);
	}
	return nullptr;
}

BulletState* BulletExplodeState::HandleInput(Bullet& bullet, Input& input)
{
	return nullptr;
}
