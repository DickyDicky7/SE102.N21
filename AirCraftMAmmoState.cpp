#include "AirCraft.h"

AirCraftMAmmoState::AirCraftMAmmoState() : AirCraftState()
{
}

AirCraftMAmmoState::~AirCraftMAmmoState()
{
}


void AirCraftMAmmoState::Exit(AirCraft& aircraft)
{
}

void AirCraftMAmmoState::Enter(AirCraft& aircraft)
{
}

void AirCraftMAmmoState::Render(AirCraft& aircraft)
{
	aircraft.SetAnimation(AIRCRAFT_ANIMATION_ID::M_AMMO, aircraft.GetPosition(), aircraft.GetMovingDirection(), aircraft.GetAngle());
}

AirCraftState* AirCraftMAmmoState::Update(AirCraft& aircraft)
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

AirCraftState* AirCraftMAmmoState::HandleInput(AirCraft& aircraft, Input& input)
{
	return NULL;
}