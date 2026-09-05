#include "Bullet.h"
#include "ParticleSystem.h"

BulletSState::BulletSState() : BulletState()
{

}

BulletSState::~BulletSState()
{
}

void BulletSState::Exit(Bullet& bullet)
{
}

void BulletSState::Enter(Bullet& bullet)
{
	Sound::GetInstance()->Play("shootS", false, 1);
}

void BulletSState::Render(Bullet& bullet)
{
	bullet.SetAnimation(BULLET_ANIMATION_ID::S, bullet.GetPosition(), bullet.GetMovingDirection(), bullet.GetAngle());
}

BulletState* BulletSState::Update(Bullet& bullet)
{
	auto resultX = Motion::CalculateUniformMotion({ bullet.GetX(), bullet.GetVX() });
	bullet.SetX(resultX.coordinate);
	auto resultY = Motion::CalculateUniformMotion({ bullet.GetY(), bullet.GetVY() });
	bullet.SetY(resultY.coordinate);
	return nullptr;
}

BulletState* BulletSState::HandleInput(Bullet& bullet, Input& input)
{
	return nullptr;
}

const std::vector<float> BulletSState::spreadDegrees{
	+Constants::Weapons::SPREAD_DEGREE_10,
	-Constants::Weapons::SPREAD_DEGREE_10,
	+Constants::Weapons::SPREAD_DEGREE_20,
	-Constants::Weapons::SPREAD_DEGREE_20
};

void BulletSState::SpawnTrail(const Bullet& bullet) const
{
	BulletParticleSystem::AddParticle(bullet.GetX(), bullet.GetY(), -bullet.GetVX() * Constants::Particles::TRAIL_DRAG_RATIO_S, -bullet.GetVY() * Constants::Particles::TRAIL_DRAG_RATIO_S, Constants::Particles::TRAIL_SIZE_S, Constants::Particles::TRAIL_LIFE_S, GraphicsHelper::ToXMFloat4(Constants::Particles::TRAIL_COLOUR_S));
}

DirectX::XMFLOAT4 BulletSState::GetExplodeColor() const
{
	return GraphicsHelper::ToXMFloat4(Constants::Particles::EXPLODE_COLOUR_S);
}

void BulletSState::SpawnBullets(float x, float y, float angle, float vx, float vy, float ax, float ay, DIRECTION movingDirection, std::vector<Bullet*>& bullets) const
{
	// Main bullet
	bullets.push_back(Bullet::Create(x, y, vx, vy, ax, ay, angle, movingDirection, false, new BulletSState()));

	// Spread bullets
	for (const auto& spreadDegree : BulletSState::spreadDegrees)
	{
		float rad = D3DXToRadian(spreadDegree);
		float cosRad = std::cos(rad);
		float sinRad = std::sin(rad);
		float svx = vx * cosRad - vy * sinRad;
		float svy = vx * sinRad + vy * cosRad;

		bullets.push_back(Bullet::Create(x, y, svx, svy, ax, ay, angle, movingDirection, false, new BulletSState()));
	}
}
