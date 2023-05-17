#include "AirCraft.h"

AirCraftNormalState::AirCraftNormalState() : AirCraftState()
{
}

AirCraftNormalState::~AirCraftNormalState()
{
}


void AirCraftNormalState::Exit(AirCraft& aircraft)
{
}

void AirCraftNormalState::Enter(AirCraft& aircraft)
{
}

void AirCraftNormalState::Render(AirCraft& aircraft)
{
	aircraft.SetAnimation(AIRCRAFT_ANIMATION_ID::NORMAL, aircraft.GetPosition(), aircraft.GetMovingDirection(), aircraft.GetAngle());
}

AirCraftState* AirCraftNormalState::Update(AirCraft& aircraft)
{
	FLOAT x = aircraft.GetX();
	FLOAT y = aircraft.GetY();
	FLOAT vx = aircraft.GetVX();
	FLOAT vy = aircraft.GetVY();

	Motion::OscillatoryMotionInputParameters pio{ y0, time, dt, T, A, φ };
	auto poo = Motion::CalculateOscillatoryMotion(pio);
	time = poo.t;

	x += vx;
	if (x >= 500.0f || x <= 0.0f) vx = -vx;
	aircraft.SetX(x);
	aircraft.SetVX(vx);

	aircraft.SetY(poo.c);

	return NULL;
}

AirCraftState* AirCraftNormalState::HandleInput(AirCraft& aircraft, Input& input)
{
	if (input.IsKey(DIK_SPACE))
	{
		return new AirCraftFAmmoState();
	}
	return NULL;
}