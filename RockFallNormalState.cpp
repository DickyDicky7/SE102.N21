#include "RockFall.h"

RockFallNormalState::RockFallNormalState() : RockFallState()
{
}

RockFallNormalState::~RockFallNormalState()
{
}


void RockFallNormalState::Exit(RockFall& RockFall)
{
}

void RockFallNormalState::Enter(RockFall& RockFall)
{
}

void RockFallNormalState::Render(RockFall& RockFall)
{
	RockFall.SetAnimation(ROCK_FALL_ANIMATION_ID::NORMAL, RockFall.GetPosition(), RockFall.GetMovingDirection(), RockFall.GetAngle());
}

RockFallState* RockFallNormalState::Update(RockFall& rockFall)
{
	// delay 2s roi fall
	FLOAT t = rockFall.getTimeDelayToFall();
	if (--t == 0)
	{
		return new RockFallFallState();
	}
	else {
		// rung lac
		if ((int)t % 5 == 0) {
			FLOAT x = rockFall.GetX();
			FLOAT vx = rockFall.GetVX();

			x += 2 * vx;
			vx = -vx;
			rockFall.SetX(x);
			rockFall.SetVX(vx);
		}
	}
	rockFall.setTimeDelayToFall(t);
	return NULL;
}

RockFallState* RockFallNormalState::HandleInput(RockFall& RockFall, Input& input)
{
	return NULL;
}