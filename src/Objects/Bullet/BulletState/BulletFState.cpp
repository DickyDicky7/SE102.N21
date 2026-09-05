#include "Bullet.h"
#include "ParticleSystem.h"

BulletFState::BulletFState(float xO, float yO, float omega) : BulletState()
{
	this->_omega  = omega;
	this->_xO = xO;
	this->_yO = yO;

	this->_r  = Constants::Weapons::BULLET_FIRE_SPIRAL_RADIUS;
	this->_deltaOmega = Constants::Weapons::BULLET_FIRE_ANGULAR_VELOCITY_DELTA;
}

BulletFState::~BulletFState()
{
}

void BulletFState::Exit(Bullet& bullet)
{
}

void BulletFState::Enter(Bullet& bullet)
{
	Sound::GetInstance()->Play("shootF", false, 1);

	if (bullet.GetVX() != 0.0f && bullet.GetVY() != 0.0f)
	{
		this->_omega = bullet.GetVX() < 0.0f ? +Constants::Weapons::BULLET_F_SPIRAL_ANGLE_135 : +Constants::Weapons::BULLET_F_SPIRAL_ANGLE_45;
		this->_omega = bullet.GetVY() < 0.0f ? -this->_omega	    : +this->_omega    ;
		bullet.SetY(bullet.GetY() - Constants::Weapons::BULLET_F_OFFSET_Y);
		bullet.SetX(bullet.GetX() + (bullet.GetVX() < 0.0f ? +Constants::Weapons::BULLET_F_OFFSET_X : -Constants::Weapons::BULLET_F_OFFSET_X));
	}
	if (bullet.GetVX() == 0.0f && bullet.GetVY() != 0.0f)
	{
		this->_omega = Constants::Weapons::BULLET_F_SPIRAL_ANGLE_90;
		bullet.SetY(bullet.GetY() - Constants::Weapons::BULLET_F_OFFSET_Y);
	}
	if (bullet.GetVX() != 0.0f && bullet.GetVY() == 0.0f)
	{
		this->_omega = bullet.GetVX() < 0.0f ? +Constants::Weapons::BULLET_F_SPIRAL_ANGLE_180 : +0.0f;
		bullet.SetX(bullet.GetX() + (bullet.GetVX() < 0.0f ? +Constants::Weapons::BULLET_F_OFFSET_X : -Constants::Weapons::BULLET_F_OFFSET_X));
	}
	this->_xO = bullet.GetX();
	this->_yO = bullet.GetY();
}

void BulletFState::Render(Bullet& bullet)
{
	bullet.SetAnimation(BULLET_ANIMATION_ID::F, bullet.GetPosition(), bullet.GetMovingDirection(), bullet.GetAngle());
}

BulletState* BulletFState::Update(Bullet& bullet)
{
	auto resultXY = Motion::CalculateUniformCircularMotion({ this->_r, this->_omega, this->_deltaOmega, this->_xO, this->_yO });
	bullet.SetX(resultXY.positionX);
	bullet.SetY(resultXY.positionY);
	this->_omega = resultXY.angleDegrees;

	if (bullet.GetVX() != 0.0f)
	{
		auto resultXO = Motion::CalculateUniformMotion({ this->_xO, bullet.GetVX() /*/ std::abs(bullet.GetVX()) * 1.0f*/ });
		this->_xO = resultXO.coordinate;
	}
	if (bullet.GetVY() != 0.0f)
	{
		auto resultYO = Motion::CalculateUniformMotion({ this->_yO, bullet.GetVY() /*/ std::abs(bullet.GetVY()) * 1.0f*/ });
		this->_yO = resultYO.coordinate;
	}

	return nullptr;
}

BulletState* BulletFState::HandleInput(Bullet& bullet, Input& input)
{
	return nullptr;
}

void BulletFState::SpawnTrail(const Bullet& bullet) const
{
	float pvx = -bullet.GetVX() * Constants::Particles::TRAIL_DRAG_RATIO + ((rand() % Constants::Particles::RANDOM_PERCENT_MODULO) - Constants::Particles::RANDOM_PERCENT_HALF) / Constants::Particles::TRAIL_F_JITTER_DIVISOR;
	float pvy = -bullet.GetVY() * Constants::Particles::TRAIL_DRAG_RATIO + ((rand() % Constants::Particles::RANDOM_PERCENT_MODULO) - Constants::Particles::RANDOM_PERCENT_HALF) / Constants::Particles::TRAIL_F_JITTER_DIVISOR;
	BulletParticleSystem::AddParticle(bullet.GetX(), bullet.GetY(), pvx, pvy, Constants::Particles::TRAIL_SIZE_F_CORE, Constants::Particles::TRAIL_LIFE_F_CORE, GraphicsHelper::ToXMFloat4(Constants::Particles::TRAIL_COLOUR_F_CORE));
	BulletParticleSystem::AddParticle(bullet.GetX() + ((rand() % Constants::Particles::TRAIL_F_POSITION_JITTER_MODULO) - Constants::Particles::TRAIL_F_POSITION_JITTER_HALF), bullet.GetY() + ((rand() % Constants::Particles::TRAIL_F_POSITION_JITTER_MODULO) - Constants::Particles::TRAIL_F_POSITION_JITTER_HALF), pvx, pvy, Constants::Particles::TRAIL_SIZE_F_SPARK, Constants::Particles::TRAIL_LIFE_F_SPARK, GraphicsHelper::ToXMFloat4(Constants::Particles::TRAIL_COLOUR_F_SPARK));
}

DirectX::XMFLOAT4 BulletFState::GetExplodeColor() const
{
	return GraphicsHelper::ToXMFloat4(Constants::Particles::EXPLODE_COLOUR_F);
}

void BulletFState::SpawnBullets(float x, float y, float angle, float vx, float vy, float ax, float ay, DIRECTION movingDirection, std::vector<Bullet*>& bullets) const
{
	bullets.push_back(Bullet::Create(x, y, vx, vy, ax, ay, angle, movingDirection, false, new BulletFState()));
}
