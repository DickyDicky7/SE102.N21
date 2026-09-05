#include "Bullet.h"

BulletState::BulletState() : State()
{
	this->_time = 0.0f;
}

BulletState::~BulletState()
{
}

void BulletState::SpawnTrail(const Bullet& bullet) const
{
}

DirectX::XMFLOAT4 BulletState::GetExplodeColor() const
{
	return GraphicsHelper::ToXMFloat4(Constants::Particles::EXPLODE_COLOUR_DEFAULT);
}

void BulletState::SpawnBullets(float x, float y, float angle, float vx, float vy, float ax, float ay, DIRECTION movingDirection, std::vector<Bullet*>& bullets) const
{
	bullets.push_back(Bullet::Create(x, y, vx, vy, ax, ay, angle, movingDirection, false, new BulletRState()));
}

BulletParticleConfig BulletState::GetParticleConfig() const
{
	return { GraphicsHelper::ToXMFloat4(Constants::Particles::GLOW_COLOUR_DEFAULT), Constants::Particles::GLOW_SIZE_DEFAULT };
}

BulletParticleConfig BulletRState::GetParticleConfig() const
{
	return { GraphicsHelper::ToXMFloat4(Constants::Particles::GLOW_COLOUR_R), Constants::Particles::GLOW_SIZE_R };
}

BulletParticleConfig BulletMState::GetParticleConfig() const
{
	return { GraphicsHelper::ToXMFloat4(Constants::Particles::GLOW_COLOUR_M), Constants::Particles::GLOW_SIZE_M };
}

BulletParticleConfig BulletSState::GetParticleConfig() const
{
	return { GraphicsHelper::ToXMFloat4(Constants::Particles::GLOW_COLOUR_S), Constants::Particles::GLOW_SIZE_S };
}

BulletParticleConfig BulletLState::GetParticleConfig() const
{
	return { GraphicsHelper::ToXMFloat4(Constants::Particles::GLOW_COLOUR_L), Constants::Particles::GLOW_SIZE_L };
}

BulletParticleConfig BulletFState::GetParticleConfig() const
{
	return { GraphicsHelper::ToXMFloat4(Constants::Particles::GLOW_COLOUR_F), Constants::Particles::GLOW_SIZE_F };
}

BulletParticleConfig BulletEnemyState::GetParticleConfig() const
{
	return { GraphicsHelper::ToXMFloat4(Constants::Particles::GLOW_COLOUR_ENEMY), Constants::Particles::GLOW_SIZE_ENEMY };
}

BulletParticleConfig BulletBossStage1State::GetParticleConfig() const
{
	return { GraphicsHelper::ToXMFloat4(Constants::Particles::GLOW_COLOUR_BOSS), Constants::Particles::GLOW_SIZE_BOSS };
}

BulletParticleConfig BulletBossStage2State::GetParticleConfig() const
{
	return { GraphicsHelper::ToXMFloat4(Constants::Particles::GLOW_COLOUR_BOSS), Constants::Particles::GLOW_SIZE_BOSS };
}

BulletParticleConfig BulletScubaSoldierState::GetParticleConfig() const
{
	return { GraphicsHelper::ToXMFloat4(Constants::Particles::GLOW_COLOUR_ENEMY), Constants::Particles::GLOW_SIZE_ENEMY };
}
