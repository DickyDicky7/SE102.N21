#include "Bullet.h"
#include "Explosion.h"
#include "ParticleSystem.h"

BulletScubaSoldierState::BulletScubaSoldierState(float theta, float v0) : BulletState()
{
	this->_theta = theta; this->_v0 = v0;
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
	if (bullet.IsFake()
	&&  bullet.GetVY() <= 0.0f)
		bullet.SetDead(true);

	if (bullet.GetVY() <= 0.0f)
	{
		if (bullet.GetAY() != 0.0f)
		{
			this->_time = 0.0f;
			bullet.SetAY(0.0f);
		}
		auto result = Motion::CalculateProjectileMotion({ bullet.GetX(), bullet.GetY(), this->_v0, 0, this->_time, Constants::Weapons::SCUBA_BULLET_PROJECTILE_DELTA_TIME });
		bullet.SetVX(result.velocityX);
		bullet.SetVY(result.velocityY);
		bullet.SetX(result.positionX);
		bullet.SetY(result.positionY);
		this->_time = result.elapsedTime;
	}
	else
	{
		auto   result = Motion::CalculateUniformlyDeceleratedMotion({ bullet.GetY(), bullet.GetVY(), bullet.GetAY(), this->_time, Constants::Weapons::SCUBA_BULLET_MOTION_DELTA_TIME });
		this->_time = result.elapsedTime; bullet.SetY(result.coordinate); bullet.SetVY(result.velocity);
	}
	return nullptr;
}

BulletState* BulletScubaSoldierState::HandleInput(Bullet& bullet, Input& input)
{
	return nullptr;
}

void BulletScubaSoldierState::SpawnTrail(const Bullet& bullet) const
{
	BulletParticleSystem::AddParticle(bullet.GetX(), bullet.GetY(), -bullet.GetVX() * Constants::Particles::TRAIL_DRAG_RATIO, -bullet.GetVY() * Constants::Particles::TRAIL_DRAG_RATIO, Constants::Particles::TRAIL_SIZE_ENEMY, Constants::Particles::TRAIL_LIFE_ENEMY, GraphicsHelper::ToXMFloat4(Constants::Particles::TRAIL_COLOUR_ENEMY));
}

void BulletScubaSoldierState::SpawnBullets(float x, float y, float angle, float vx, float vy, float ax, float ay, DIRECTION movingDirection, std::vector<Bullet*>& bullets) const
{
	bullets.push_back(Bullet::Create(x, y, vx, vy - Constants::Enemies::ScubaSoldier::SPREAD_LEAD_BULLET_SPEED_Y_OFFSET, ax, ay, angle, movingDirection, true, new BulletScubaSoldierState(+Constants::Enemies::ScubaSoldier::SPREAD_ANGLE_STRAIGHT_DEGREES, +Constants::Enemies::ScubaSoldier::SPREAD_STRAIGHT_DRIFT_X), true));
	bullets.push_back(Bullet::Create(x, y, vx, vy, ax, ay, angle, movingDirection, true, new BulletScubaSoldierState(+Constants::Enemies::ScubaSoldier::SPREAD_ANGLE_FANNED_DEGREES, -Constants::Enemies::ScubaSoldier::SPREAD_FANNED_DRIFT_X)));
	bullets.push_back(Bullet::Create(x, y, vx, vy, ax, ay, angle, movingDirection, true, new BulletScubaSoldierState(+Constants::Enemies::ScubaSoldier::SPREAD_ANGLE_FANNED_DEGREES, +Constants::Enemies::ScubaSoldier::SPREAD_FANNED_DRIFT_X)));
	bullets.push_back(Bullet::Create(x, y, vx, vy, ax, ay, angle, movingDirection, true, new BulletScubaSoldierState(+Constants::Enemies::ScubaSoldier::SPREAD_ANGLE_STRAIGHT_DEGREES, +Constants::Enemies::ScubaSoldier::SPREAD_STRAIGHT_DRIFT_X)));
}

Explosion* BulletScubaSoldierState::CreateDeathExplosion(const Bullet& bullet) const
{
	Explosion* explosion = nullptr;
	if (bullet.IsFake()) explosion = new Explosion(new ExplosionType2State());
	else explosion = new Explosion(new ExplosionType1State());
	explosion->SetX(bullet.GetX());
	explosion->SetY(bullet.GetY() - Constants::Weapons::SCUBA_BULLET_EXPLOSION_OFFSET_Y);
	return explosion;
}

void BulletScubaSoldierState::OnTerrainCollision(Bullet& bullet, TERRAIN_BLOCK_TYPE terrainType, float normalY) const
{
	if (normalY == +1.0f && (terrainType == TERRAIN_BLOCK_TYPE::THROUGHABLE || terrainType == TERRAIN_BLOCK_TYPE::NON_THROUGHABLE))
	{
		bullet.SetDead(true);
	}
}
