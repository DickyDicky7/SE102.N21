#include "RockFall.h"

RockFallFallState::RockFallFallState() : RockFallState()
{
}

RockFallFallState::~RockFallFallState()
{
}


void RockFallFallState::Exit(RockFall& RockFall)
{
}

void RockFallFallState::Enter(RockFall& RockFall)
{
}

void RockFallFallState::Render(RockFall& RockFall)
{
	RockFall.SetAnimation(ROCK_FALL_ANIMATION_ID::FALL, RockFall.GetPosition(), RockFall.GetMovingDirection(), RockFall.GetAngle());
}

RockFallState* RockFallFallState::Update(RockFall& rockFall)
{
	FLOAT y = rockFall.GetY();
	FLOAT vy = rockFall.GetVY();

	y -= vy;

	rockFall.SetY(y);
	return NULL;
}

RockFallState* RockFallFallState::HandleInput(RockFall& RockFall, Input& input)
{
	return NULL;
}