#include "Explosion.h"

ExplosionDrownState::ExplosionDrownState() : ExplosionState()
{
	this->_time = Constants::Objects::Explosion::DROWN_DURATION_FRAMES;
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
	if (--this->_time == 0)
	{
		explosion.SetDead(true);
	}
	return nullptr;
}

ExplosionState* ExplosionDrownState::HandleInput(Explosion& explosion, Input& input)
{
	return nullptr;
}
