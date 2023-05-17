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
	// Chuyen dong hinh sin
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
	//invul, b, f, l, m, r, s
	if (input.IsKey(DIK_1))
	{
		return new AirCraftInvulState();
	} 
	else if (input.IsKey(DIK_2))
	{
		return new AirCraftBAmmoState();
	}
	else if (input.IsKey(DIK_3))
	{
		return new AirCraftFAmmoState();
	}
	else if (input.IsKey(DIK_4))
	{
		return new AirCraftLAmmoState();
	}
	else if (input.IsKey(DIK_5))
	{
		return new AirCraftMAmmoState();
	}
	else if (input.IsKey(DIK_6))
	{
		return new AirCraftRAmmoState();
	}
	else if (input.IsKey(DIK_7))
	{
		return new AirCraftSAmmoState();
	}
	
	return NULL;
}