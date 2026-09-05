#include "Bullet.h"
#include "ParticleSystem.h"

BulletBossStage2State::BulletBossStage2State() : BulletState()
{
	Sound::GetInstance()->Play("boss2bulletsound", false, 1);
}

BulletBossStage2State::~BulletBossStage2State()
{
}

void BulletBossStage2State::Exit(Bullet& bullet)
{
}

void BulletBossStage2State::Enter(Bullet& bullet)
{
}

void BulletBossStage2State::Render(Bullet& bullet)
{
	bullet.SetAnimation(BULLET_ANIMATION_ID::BOSS_2, bullet.GetPosition(), bullet.GetMovingDirection(), bullet.GetAngle());
}

BulletState* BulletBossStage2State::Update(Bullet& bullet)
{
	auto resultX = Motion::CalculateUniformMotion({ bullet.GetX(), bullet.GetVX() });
	bullet.SetX(resultX.coordinate);
	auto resultY = Motion::CalculateUniformMotion({ bullet.GetY(), bullet.GetVY() });
	bullet.SetY(resultY.coordinate);
	return nullptr;
}

BulletState* BulletBossStage2State::HandleInput(Bullet& bullet, Input& input)
{
	return nullptr;
}

void BulletBossStage2State::SpawnTrail(const Bullet& bullet) const
{
	BulletParticleSystem::AddParticle(bullet.GetX(), bullet.GetY(), -bullet.GetVX() * Constants::Particles::TRAIL_DRAG_RATIO, -bullet.GetVY() * Constants::Particles::TRAIL_DRAG_RATIO, Constants::Particles::TRAIL_SIZE_BOSS, Constants::Particles::TRAIL_LIFE_BOSS, GraphicsHelper::ToXMFloat4(Constants::Particles::TRAIL_COLOUR_BOSS));
}

DirectX::XMFLOAT4 BulletBossStage2State::GetExplodeColor() const
{
	return GraphicsHelper::ToXMFloat4(Constants::Particles::EXPLODE_COLOUR_BOSS);
}

void BulletBossStage2StateHand::SpawnBullets(float x, float y, float angle, float vx, float vy, float ax, float ay, DIRECTION movingDirection, std::vector<Bullet*>& bullets) const
{
	bullets.push_back(Bullet::Create(x, y, vx, vy, ax, ay, angle, movingDirection, true, new BulletBossStage2StateHand()));
}
