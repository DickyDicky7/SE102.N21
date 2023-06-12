#include "Explosion.h"

// state cho khong phai nguoi
ExplosionType2State::ExplosionType2State() : ExplosionState()
{
	delayTime = 100.0f;
}

ExplosionType2State::~ExplosionType2State()
{
	delayTime = NULL;
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
	if (delayTime-- < 0) {
		// delete state
	}
	return NULL;
}

ExplosionState* ExplosionType2State::HandleInput(Explosion& explosion, Input& input)
{
	return NULL;
}