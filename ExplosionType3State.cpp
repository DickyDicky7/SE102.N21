#include "Explosion.h"

// state cho khong phai nguoi
ExplosionType3State::ExplosionType3State() : ExplosionState()
{
	delayTime = 100.0f;
}

ExplosionType3State::~ExplosionType3State()
{
	delayTime = NULL;
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
	if (delayTime-- < 0) {
		explosion.isDead = 1;
	}
	return NULL;
}

ExplosionState* ExplosionType3State::HandleInput(Explosion& explosion, Input& input)
{
	return NULL;
}