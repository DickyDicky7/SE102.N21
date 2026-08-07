#include "RockFall.h"

RockFallFallState::RockFallFallState() : RockFallState()
{
	Sound::getInstance()->play("stonefailing", false, 1);
}

RockFallFallState::~RockFallFallState()
{
}


void RockFallFallState::Exit(RockFall& RockFall)
{
}

void RockFallFallState::Enter(RockFall& RockFall)
{
	RockFall.SetVY(-abs(RockFall.GetVY()));
}

void RockFallFallState::Render(RockFall& RockFall)
{
	RockFall.SetAnimation(ROCK_FALL_ANIMATION_ID::FALL, RockFall.GetPosition(), RockFall.GetMovingDirection(), RockFall.GetAngle());
}

RockFallState* RockFallFallState::Update(RockFall& rockFall)
{
	if (rockFall.bouncedBack)
	{
		auto   result = Motion::CalculateUniformlyDeceleratedMotion({ rockFall.GetY(), rockFall.GetVY(), rockFall.GetAY(), time, 0.05f });
		time = result.t; rockFall.SetY(result.c); rockFall.SetVY(result.v);
		if (rockFall.GetVY() <= 0.0f)
		{
			rockFall.bouncedBack = 0;
			rockFall.SetVY(-1.0f);
		}
	}
	else
	{
		auto result = Motion::CalculateUniformMotion({ rockFall.GetY(), rockFall.GetVY() });
		rockFall.SetY(result.c);
	}
	return NULL;
}

RockFallState* RockFallFallState::HandleInput(RockFall& RockFall, Input& input)
{
	return NULL;
}