#include "Bullet.h"
#include "ParticleSystem.h"

BulletMState::BulletMState() : BulletState()
{

}

BulletMState::~BulletMState()
{
}

void BulletMState::Exit(Bullet& bullet)
{
}

void BulletMState::Enter(Bullet& bullet)
{
	Sound::GetInstance()->Play("shootM", false, 1);
}

void BulletMState::Render(Bullet& bullet)
{
	bullet.SetAnimation(BULLET_ANIMATION_ID::M, bullet.GetPosition(), bullet.GetMovingDirection(), bullet.GetAngle());
}

BulletState* BulletMState::Update(Bullet& bullet)
{
	auto resultX = Motion::CalculateUniformMotion({ bullet.GetX(), bullet.GetVX() });
	bullet.SetX(resultX.coordinate);
	auto resultY = Motion::CalculateUniformMotion({ bullet.GetY(), bullet.GetVY() });
	bullet.SetY(resultY.coordinate);
	return nullptr;
}

BulletState* BulletMState::HandleInput(Bullet& bullet, Input& input)
{
	return nullptr;
}

void BulletMState::SpawnTrail(const Bullet& bullet) const
{
	BulletParticleSystem::AddParticle(bullet.GetX(), bullet.GetY(), -bullet.GetVX() * Constants::Particles::TRAIL_DRAG_RATIO_M, -bullet.GetVY() * Constants::Particles::TRAIL_DRAG_RATIO_M, Constants::Particles::TRAIL_SIZE_M, Constants::Particles::TRAIL_LIFE_M, GraphicsHelper::ToXMFloat4(Constants::Particles::TRAIL_COLOUR_M));
}

DirectX::XMFLOAT4 BulletMState::GetExplodeColor() const
{
	return GraphicsHelper::ToXMFloat4(Constants::Particles::EXPLODE_COLOUR_M);
}

void BulletMState::SpawnBullets(float x, float y, float angle, float vx, float vy, float ax, float ay, DIRECTION movingDirection, std::vector<Bullet*>& bullets) const
{
	bullets.push_back(Bullet::Create(x, y, vx, vy, ax, ay, angle, movingDirection, false, new BulletMState()));
}
