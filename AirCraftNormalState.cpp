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
	
	aircraft.SetX(x);
	aircraft.SetY(poo.c);

	return NULL;
}

AirCraftState* AirCraftNormalState::HandleInput(AirCraft& aircraft, Input& input)
{
	//invul, b, f, l, m, r, s
	if (input.IsKey(DIK_1)) {
		if (aircraft.getAmmoType() == AMMO_TYPE::I)
		{
			return new AirCraftInvulState();
		}
		else if (aircraft.getAmmoType() == AMMO_TYPE::B)
		{
			return new AirCraftBAmmoState();
		}
		else if (aircraft.getAmmoType() == AMMO_TYPE::F)
		{
			return new AirCraftFAmmoState();
		}
		else if (aircraft.getAmmoType() == AMMO_TYPE::L)
		{
			return new AirCraftLAmmoState();
		}
		else if (aircraft.getAmmoType() == AMMO_TYPE::M)
		{
			return new AirCraftMAmmoState();
		}
		else if (aircraft.getAmmoType() == AMMO_TYPE::R)
		{
			return new AirCraftRAmmoState();
		}
		else if (aircraft.getAmmoType() == AMMO_TYPE::S)
		{
			return new AirCraftSAmmoState();
		}
	}
	
	
	return NULL;
}