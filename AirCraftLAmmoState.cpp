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
	return NULL;
}

AirCraftState* AirCraftLAmmoState::HandleInput(AirCraft& aircraft, Input& input)
{
	return NULL;
}