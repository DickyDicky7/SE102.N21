#include "Bullet.h"
#include "ParticleSystem.h"

BulletRState::BulletRState() : BulletState()
{

}

BulletRState::~BulletRState()
{
}

void BulletRState::Exit(Bullet& bullet)
{
}

void BulletRState::Enter(Bullet& bullet)
{
	Sound::GetInstance()->Play("shootM", false, 1);
}

void BulletRState::Render(Bullet& bullet)
{
	bullet.SetAnimation(BULLET_ANIMATION_ID::R, bullet.GetPosition(), bullet.GetMovingDirection(), bullet.GetAngle());
}

BulletState* BulletRState::Update(Bullet& bullet)
{
	auto resultX = Motion::CalculateUniformMotion({ bullet.GetX(), bullet.GetVX() });
	bullet.SetX(resultX.coordinate);
	auto resultY = Motion::CalculateUniformMotion({ bullet.GetY(), bullet.GetVY() });
	bullet.SetY(resultY.coordinate);
	return nullptr;
}

BulletState* BulletRState::HandleInput(Bullet& bullet, Input& input)
{
	return nullptr;
}

void BulletRState::SpawnTrail(const Bullet& bullet) const
{
	float vx_offset = (((rand() % Constants::Particles::RANDOM_PERCENT_MODULO) - Constants::Particles::RANDOM_PERCENT_HALF) / Constants::Particles::TRAIL_R_JITTER_DIVISOR) * Constants::Particles::TRAIL_R_JITTER_SCALE;
	float vy_offset = (((rand() % Constants::Particles::RANDOM_PERCENT_MODULO) - Constants::Particles::RANDOM_PERCENT_HALF) / Constants::Particles::TRAIL_R_JITTER_DIVISOR) * Constants::Particles::TRAIL_R_JITTER_SCALE;
	BulletParticleSystem::AddParticle(bullet.GetX(), bullet.GetY(), -bullet.GetVX() * Constants::Particles::TRAIL_DRAG_RATIO + vx_offset, -bullet.GetVY() * Constants::Particles::TRAIL_DRAG_RATIO + vy_offset, Constants::Particles::TRAIL_SIZE_R, Constants::Particles::TRAIL_LIFE_R, GraphicsHelper::ToXMFloat4(Constants::Particles::TRAIL_COLOUR_R));
}

DirectX::XMFLOAT4 BulletRState::GetExplodeColor() const
{
	return GraphicsHelper::ToXMFloat4(Constants::Particles::EXPLODE_COLOUR_DEFAULT);
}

void BulletRState::SpawnBullets(float x, float y, float angle, float vx, float vy, float ax, float ay, DIRECTION movingDirection, std::vector<Bullet*>& bullets) const
{
	bullets.push_back(Bullet::Create(x, y, vx, vy, ax, ay, angle, movingDirection, false, new BulletRState()));
}
