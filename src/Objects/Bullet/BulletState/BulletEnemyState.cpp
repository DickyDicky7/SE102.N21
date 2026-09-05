#include "Bullet.h"
#include "ParticleSystem.h"

BulletEnemyState::BulletEnemyState() : BulletState()
{
	Sound::GetInstance()->Play("enemydead", false, 1);
}

BulletEnemyState::~BulletEnemyState()
{
}

void BulletEnemyState::Exit(Bullet& bullet)
{
}

void BulletEnemyState::Enter(Bullet& bullet)
{
}

void BulletEnemyState::Render(Bullet& bullet)
{
	bullet.SetAnimation(BULLET_ANIMATION_ID::ENEMY, bullet.GetPosition(), bullet.GetMovingDirection(), bullet.GetAngle());
}

BulletState* BulletEnemyState::Update(Bullet& bullet)
{
	auto resultX = Motion::CalculateUniformMotion({ bullet.GetX(), bullet.GetVX() });
	bullet.SetX(resultX.coordinate);
	auto resultY = Motion::CalculateUniformMotion({ bullet.GetY(), bullet.GetVY() });
	bullet.SetY(resultY.coordinate);
	return nullptr;
}

BulletState* BulletEnemyState::HandleInput(Bullet& bullet, Input& input)
{
	return nullptr;
}

void BulletEnemyState::SpawnTrail(const Bullet& bullet) const
{
	BulletParticleSystem::AddParticle(bullet.GetX(), bullet.GetY(), -bullet.GetVX() * Constants::Particles::TRAIL_DRAG_RATIO, -bullet.GetVY() * Constants::Particles::TRAIL_DRAG_RATIO, Constants::Particles::TRAIL_SIZE_ENEMY, Constants::Particles::TRAIL_LIFE_ENEMY, GraphicsHelper::ToXMFloat4(Constants::Particles::TRAIL_COLOUR_ENEMY));
}

void BulletEnemyState::SpawnBullets(float x, float y, float angle, float vx, float vy, float ax, float ay, DIRECTION movingDirection, std::vector<Bullet*>& bullets) const
{
	bullets.push_back(Bullet::Create(x, y, vx, vy, ax, ay, angle, movingDirection, true, new BulletEnemyState()));
}
