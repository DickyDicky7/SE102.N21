#include "AirCraft.h"

AirCraftFAmmoState::AirCraftFAmmoState() : AirCraftState()
{
}

AirCraftFAmmoState::~AirCraftFAmmoState()
{
}


void AirCraftFAmmoState::Exit(AirCraft& aircraft)
{
}

void AirCraftFAmmoState::Enter(AirCraft& aircraft)
{
}

void AirCraftFAmmoState::Render(AirCraft& aircraft)
{
	aircraft.SetAnimation(AIRCRAFT_ANIMATION_ID::F_AMMO, aircraft.GetPosition(), aircraft.GetMovingDirection(), aircraft.GetAngle());
}

AirCraftState* AirCraftFAmmoState::Update(AirCraft& aircraft)
{
	// chuyen dong nem xien
	FLOAT x = aircraft.GetX();
	FLOAT y = aircraft.GetY();

	Motion::ProjectileMotionInputParameters pip{ x, y, v0, θ, time, dt };

	if (y >= aircraft.GetH() / 2 ) {
		auto pop = Motion::CalculateProjectileMotion(pip);
		aircraft.SetX(pop.x);
		aircraft.SetY(pop.y);
		time = pop.t;
	}
	return NULL;
}

AirCraftState* AirCraftFAmmoState::HandleInput(AirCraft& aircraft, Input& input)
{
	return NULL;
}