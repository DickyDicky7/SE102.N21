#include "Bullet.h"
#include "Explosion.h"
#include "ParticleSystem.h"

BulletBossStage1State::BulletBossStage1State() : BulletState()
{
	Sound::GetInstance()->Play("boss2bulletsound", false, 1);
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
	auto result = Motion::CalculateProjectileMotion({ bullet.GetX(), bullet.GetY(), bullet.GetAngle() <= Constants::Weapons::BULLET_BOSS_MAX_LAUNCH_ANGLE ? bullet.GetAngle() : Constants::Weapons::BULLET_BOSS_MAX_LAUNCH_ANGLE, +Constants::Weapons::BULLET_BOSS_PROJECTILE_ANGLE_DEGREES, this->_time, Constants::Weapons::BULLET_BOSS_PROJECTILE_DELTA_TIME });
	bullet.SetVX(result.velocityX);
	bullet.SetVY(result.velocityY);
	bullet.SetX(result.positionX);
	bullet.SetY(result.positionY);
	this->_time = result.elapsedTime;
	return nullptr;
}

BulletState* BulletBossStage1State::HandleInput(Bullet& bullet, Input& input)
{
	return nullptr;
}

void BulletBossStage1State::SpawnTrail(const Bullet& bullet) const
{
	BulletParticleSystem::AddParticle(bullet.GetX(), bullet.GetY(), -bullet.GetVX() * Constants::Particles::TRAIL_DRAG_RATIO, -bullet.GetVY() * Constants::Particles::TRAIL_DRAG_RATIO, Constants::Particles::TRAIL_SIZE_BOSS, Constants::Particles::TRAIL_LIFE_BOSS, GraphicsHelper::ToXMFloat4(Constants::Particles::TRAIL_COLOUR_BOSS));
}

DirectX::XMFLOAT4 BulletBossStage1State::GetExplodeColor() const
{
	return GraphicsHelper::ToXMFloat4(Constants::Particles::EXPLODE_COLOUR_BOSS);
}

void BulletBossStage1State::SpawnBullets(float x, float y, float angle, float vx, float vy, float ax, float ay, DIRECTION movingDirection, std::vector<Bullet*>& bullets) const
{
	bullets.push_back(Bullet::Create(x, y, vx, vy, ax, ay, angle, movingDirection, true, new BulletBossStage1State()));
}

Explosion* BulletBossStage1State::CreateDeathExplosion(const Bullet& bullet) const
{
	Explosion* explosion = new Explosion(new ExplosionType2State());
	explosion->SetX(bullet.GetX());
	explosion->SetY(bullet.GetY() - Constants::Weapons::BULLET_BOSS_EXPLOSION_OFFSET_Y);
	return explosion;
}

void BulletBossStage1State::OnTerrainCollision(Bullet& bullet, TERRAIN_BLOCK_TYPE terrainType, float normalY) const
{
	if (normalY == +1.0f && terrainType == TERRAIN_BLOCK_TYPE::NON_THROUGHABLE)
	{
		bullet.SetDead(true);
	}
}

