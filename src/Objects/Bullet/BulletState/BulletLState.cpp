#include "Bullet.h"
#include "ParticleSystem.h"

BulletLState::BulletLState() : BulletState()
{

}

BulletLState::~BulletLState()
{
}

void BulletLState::Exit(Bullet& bullet)
{
}

void BulletLState::Enter(Bullet& bullet)
{
	Sound::GetInstance()->Play("shootL", false, 1);

	if (bullet.GetVX() != 0.0f && bullet.GetVY() != 0.0f)
	{
		bullet.SetY(bullet.GetY() + Constants::Weapons::BULLET_L_OFFSET_Y_UP);
		bullet.SetAngle(bullet.GetVX() < 0.0f ? +Constants::Weapons::BULLET_L_ANGLE_135 : +Constants::Weapons::BULLET_L_ANGLE_45);
		bullet.SetAngle(bullet.GetVY() < 0.0f ? -bullet.GetAngle() : +bullet.GetAngle());
	}
	if (bullet.GetVX() == 0.0f && bullet.GetVY() != 0.0f)
	{
		bullet.SetAngle(Constants::Weapons::BULLET_L_ANGLE_90);
		bullet.SetX(bullet.GetX() + Constants::Weapons::BULLET_L_OFFSET_X);
	}
	if (bullet.GetVX() != 0.0f && bullet.GetVY() == 0.0f)
	{
		bullet.SetY(bullet.GetY() - Constants::Weapons::BULLET_L_OFFSET_Y_DOWN);
	}
}

void BulletLState::Render(Bullet& bullet)
{
	bullet.SetAnimation(BULLET_ANIMATION_ID::L, bullet.GetPosition(), bullet.GetMovingDirection(), bullet.GetAngle());
}

BulletState* BulletLState::Update(Bullet& bullet)
{
	auto resultX = Motion::CalculateUniformMotion({ bullet.GetX(), bullet.GetVX() });
	bullet.SetX(resultX.coordinate);
	auto resultY = Motion::CalculateUniformMotion({ bullet.GetY(), bullet.GetVY() });
	bullet.SetY(resultY.coordinate);
	return nullptr;
}

BulletState* BulletLState::HandleInput(Bullet& bullet, Input& input)
{
	return nullptr;
}

void BulletLState::SpawnTrail(const Bullet& bullet) const
{
	float startX = bullet.GetX() - bullet.GetVX();
	float startY = bullet.GetY() - bullet.GetVY();
	for (int i = 0; i < Constants::Particles::TRAIL_L_SEGMENT_COUNT; ++i)
	{
		float t = static_cast<float>(i) / Constants::Particles::TRAIL_L_SEGMENT_DIVISOR;
		BulletParticleSystem::AddParticle(startX + bullet.GetVX() * t, startY + bullet.GetVY() * t, 0.0f, 0.0f, Constants::Particles::TRAIL_SIZE_L, Constants::Particles::TRAIL_LIFE_L, GraphicsHelper::ToXMFloat4(Constants::Particles::TRAIL_COLOUR_L));
	}
}

DirectX::XMFLOAT4 BulletLState::GetExplodeColor() const
{
	return GraphicsHelper::ToXMFloat4(Constants::Particles::EXPLODE_COLOUR_L);
}

void BulletLState::SpawnBullets(float x, float y, float angle, float vx, float vy, float ax, float ay, DIRECTION movingDirection, std::vector<Bullet*>& bullets) const
{
	bullets.push_back(Bullet::Create(x, y, vx, vy, ax, ay, angle, movingDirection, false, new BulletLState()));
}

