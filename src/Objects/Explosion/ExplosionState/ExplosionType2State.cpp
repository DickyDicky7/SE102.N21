#include "Explosion.h"

// state cho khong phai nguoi
ExplosionType2State::ExplosionType2State() : ExplosionState()
{
}

ExplosionType2State::~ExplosionType2State()
{
}


void ExplosionType2State::Exit(Explosion& explosion)
{
}

void ExplosionType2State::Enter(Explosion& explosion)
{
}

void ExplosionType2State::Render(Explosion& explosion)
{
	explosion.SetAnimation(EXPLOSION_ANIMATION_ID::TYPE_2, explosion.GetPosition(), explosion.GetMovingDirection(), explosion.GetAngle());
}

ExplosionState* ExplosionType2State::Update(Explosion& explosion)
{
	std::vector<std::tuple<SPRITE_ID, TIME>>& frames = std::get<
		std::vector<std::tuple<SPRITE_ID, TIME>>>(GraphicsDatabase::animations[EXPLOSION_ANIMATION_ID::TYPE_2]);

	if (std::cmp_greater_equal(explosion.GetCurrentFrame() + 1, frames.size()))
	{
		explosion.isDead = 1;
	}

	return NULL;
}

ExplosionState* ExplosionType2State::HandleInput(Explosion& explosion, Input& input)
{
	return NULL;
}