#include "RockFall.h"

RockFallFallState::RockFallFallState() : RockFallState()
{
	Sound::GetInstance()->Play("stonefailing", false, 1);
}

RockFallFallState::~RockFallFallState()
{
}


void RockFallFallState::Exit(RockFall& rockFall)
{
}

void RockFallFallState::Enter(RockFall& rockFall)
{
	rockFall.SetVY(-std::abs(rockFall.GetVY()));
}

void RockFallFallState::Render(RockFall& rockFall)
{
	rockFall.SetAnimation(ROCK_FALL_ANIMATION_ID::FALL, rockFall.GetPosition(), rockFall.GetMovingDirection(), rockFall.GetAngle());
}

RockFallState* RockFallFallState::Update(RockFall& rockFall)
{
	if (rockFall.HasBouncedBack())
	{
		auto   result = Motion::CalculateUniformlyDeceleratedMotion({ rockFall.GetY(), rockFall.GetVY(), rockFall.GetAY(), this->_time, Constants::Enemies::RockFall::MOTION_INTEGRATION_DELTA_TIME });
		this->_time = result.elapsedTime; rockFall.SetY(result.coordinate); rockFall.SetVY(result.velocity);
		if (rockFall.GetVY() <= 0.0f)
		{
			rockFall.SetBouncedBack(false);
			rockFall.SetVY(Constants::Enemies::RockFall::BOUNCE_SPEED_Y);
		}
	}
	else
	{
		auto result = Motion::CalculateUniformMotion({ rockFall.GetY(), rockFall.GetVY() });
		rockFall.SetY(result.coordinate);
	}
	return nullptr;
}

RockFallState* RockFallFallState::HandleInput(RockFall& rockFall, Input& input)
{
	return nullptr;
}
