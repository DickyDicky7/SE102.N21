#include "AirCraft.h"

AirCraftSAmmoState::AirCraftSAmmoState() : AirCraftState()
{
}

AirCraftSAmmoState::~AirCraftSAmmoState()
{
}


void AirCraftSAmmoState::Exit(AirCraft& aircraft)
{
}

void AirCraftSAmmoState::Enter(AirCraft& aircraft)
{
}

void AirCraftSAmmoState::Render(AirCraft& aircraft)
{
	aircraft.SetAnimation(AIRCRAFT_ANIMATION_ID::S_AMMO, aircraft.GetPosition(), aircraft.GetMovingDirection(), aircraft.GetAngle());
}

AirCraftState* AirCraftSAmmoState::Update(AirCraft& aircraft)
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

AirCraftState* AirCraftSAmmoState::HandleInput(AirCraft& aircraft, Input& input)
{
	return NULL;
}