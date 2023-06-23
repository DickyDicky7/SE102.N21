#include "Explosion.h"

ExplosionDrownState::ExplosionDrownState() : ExplosionState()
{
	time = 100.0f;
}

ExplosionDrownState::~ExplosionDrownState()
{
}

void ExplosionDrownState::Exit(Explosion& explosion)
{
}

void ExplosionDrownState::Enter(Explosion& explosion)
{
}

void ExplosionDrownState::Render(Explosion& explosion)
{
	explosion.SetAnimation(BULLET_ANIMATION_ID::BURST, explosion.GetPosition(), explosion.GetMovingDirection(), explosion.GetAngle());
}

ExplosionState* ExplosionDrownState::Update(Explosion& explosion)
{
	if (--time == 0)
	{
		explosion.isDead = 1;
	}
	return NULL;
}

ExplosionState* ExplosionDrownState::HandleInput(Explosion& explosion, Input& input)
{
	return NULL;
}