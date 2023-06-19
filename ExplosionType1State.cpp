#include "Explosion.h"

// state cho nguoi
ExplosionType1State::ExplosionType1State() : ExplosionState()
{
}

ExplosionType1State::~ExplosionType1State()
{
}


void ExplosionType1State::Exit(Explosion& explosion)
{
}

void ExplosionType1State::Enter(Explosion& explosion)
{
}

void ExplosionType1State::Render(Explosion& explosion)
{
	explosion.SetAnimation(EXPLOSION_ANIMATION_ID::TYPE_1, explosion.GetPosition(), explosion.GetMovingDirection(), explosion.GetAngle());
}

ExplosionState* ExplosionType1State::Update(Explosion& explosion)
{
	std::vector<std::tuple<SPRITE_ID, TIME>>& frames = std::get<
		std::vector<std::tuple<SPRITE_ID, TIME>>>(GraphicsDatabase::animations[EXPLOSION_ANIMATION_ID::TYPE_1]);

	if (std::cmp_greater_equal(explosion.GetCurrentFrame() + 1, frames.size()))
	{
		explosion.isDead = 1;
	}

	return NULL;
}

ExplosionState* ExplosionType1State::HandleInput(Explosion& explosion, Input& input)
{
	return NULL;
}