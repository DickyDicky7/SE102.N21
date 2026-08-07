#include "AirCraft.h"

AirCraftRAmmoState::AirCraftRAmmoState() : AirCraftState()
{
}

AirCraftRAmmoState::~AirCraftRAmmoState()
{
}


void AirCraftRAmmoState::Exit(AirCraft& aircraft)
{
}

void AirCraftRAmmoState::Enter(AirCraft& aircraft)
{
}

void AirCraftRAmmoState::Render(AirCraft& aircraft)
{
	aircraft.SetAnimation(AIRCRAFT_ANIMATION_ID::R_AMMO, aircraft.GetPosition(), aircraft.GetMovingDirection(), aircraft.GetAngle());
}

AirCraftState* AirCraftRAmmoState::Update(AirCraft& aircraft)
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

AirCraftState* AirCraftRAmmoState::HandleInput(AirCraft& aircraft, Input& input)
{
	return NULL;
}