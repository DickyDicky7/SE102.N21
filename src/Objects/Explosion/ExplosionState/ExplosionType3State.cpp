#include "Explosion.h"

// state cho khong phai nguoi
ExplosionType3State::ExplosionType3State() : ExplosionState()
{
}

ExplosionType3State::~ExplosionType3State()
{
}


void ExplosionType3State::Exit(Explosion& explosion)
{
}

void ExplosionType3State::Enter(Explosion& explosion)
{
}

void ExplosionType3State::Render(Explosion& explosion)
{
	explosion.SetAnimation(EXPLOSION_ANIMATION_ID::TYPE_3, explosion.GetPosition(), explosion.GetMovingDirection(), explosion.GetAngle());
}

ExplosionState* ExplosionType3State::Update(Explosion& explosion)
{
	std::vector<std::tuple<SPRITE_ID, TIME>>& frames = std::get<
		std::vector<std::tuple<SPRITE_ID, TIME>>>(GraphicsDatabase::animations[EXPLOSION_ANIMATION_ID::TYPE_3]);

	if (std::cmp_greater_equal(explosion.GetCurrentFrame() + 1, frames.size()))
	{
		explosion.isDead = 1;
	}

	return NULL;
}

ExplosionState* ExplosionType3State::HandleInput(Explosion& explosion, Input& input)
{
	return NULL;
}