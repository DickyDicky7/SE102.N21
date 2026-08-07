#include "AirCraft.h"

AirCraftLAmmoState::AirCraftLAmmoState() : AirCraftState()
{
}

AirCraftLAmmoState::~AirCraftLAmmoState()
{
}


void AirCraftLAmmoState::Exit(AirCraft& aircraft)
{
}

void AirCraftLAmmoState::Enter(AirCraft& aircraft)
{
}

void AirCraftLAmmoState::Render(AirCraft& aircraft)
{
	aircraft.SetAnimation(AIRCRAFT_ANIMATION_ID::L_AMMO, aircraft.GetPosition(), aircraft.GetMovingDirection(), aircraft.GetAngle());
}

AirCraftState* AirCraftLAmmoState::Update(AirCraft& aircraft)
{
	// chuyen dong nem xien
	FLOAT x = aircraft.GetX();
	FLOAT y = aircraft.GetY();

	Motion::ProjectileMotionInputParameters pip{ x, y, v0, θ, time, dt };

	if (y >= aircraft.GetH() / 2) {
		auto pop = Motion::CalculateProjectileMotion(pip);
		aircraft.SetX(pop.x);
		aircraft.SetY(pop.y);
		time = pop.t;
	}
	return NULL;
}

AirCraftState* AirCraftLAmmoState::HandleInput(AirCraft& aircraft, Input& input)
{
	return NULL;
}