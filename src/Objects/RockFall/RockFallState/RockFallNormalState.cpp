#include "RockFall.h"

RockFallNormalState::RockFallNormalState() : RockFallState()
{
}

RockFallNormalState::~RockFallNormalState()
{
}


void RockFallNormalState::Exit(RockFall& rockFall)
{
}

void RockFallNormalState::Enter(RockFall& rockFall)
{
}

void RockFallNormalState::Render(RockFall& rockFall)
{
	rockFall.SetAnimation(ROCK_FALL_ANIMATION_ID::NORMAL, rockFall.GetPosition(), rockFall.GetMovingDirection(), rockFall.GetAngle());
}

RockFallState* RockFallNormalState::Update(RockFall& rockFall)
{
	// Delay ~2s, then start falling
	float t = rockFall.GetTimeDelayToFall();
	if (--t == 0)
	{
		return new RockFallFallState();
	}
	else {
		// Shake in place
		if (static_cast<int>(t) % Constants::Enemies::RockFall::SHAKE_FRAME_MODULO == 0) {
			float x = rockFall.GetX();
			float vx = rockFall.GetVX();

			x += Constants::Enemies::RockFall::SHAKE_DISPLACEMENT_FACTOR * vx;
			vx = -vx;
			rockFall.SetX(x);
			rockFall.SetVX(vx);
		}
	}
	rockFall.SetTimeDelayToFall(t);
	return nullptr;
}

RockFallState* RockFallNormalState::HandleInput(RockFall& rockFall, Input& input)
{
	return nullptr;
}
