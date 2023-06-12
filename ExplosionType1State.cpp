#include "Explosion.h"

// state cho nguoi
ExplosionType1State::ExplosionType1State() : ExplosionState()
{
	delayTime = 100.0f;
}

ExplosionType1State::~ExplosionType1State()
{
	delayTime = NULL;
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
	if (delayTime-- < 0) {
		// delete state
	}
	return NULL;
}

ExplosionState* ExplosionType1State::HandleInput(Explosion& explosion, Input& input)
{
	return NULL;
}