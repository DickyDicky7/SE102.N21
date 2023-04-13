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
	return NULL;
}

AirCraftState* AirCraftRAmmoState::HandleInput(AirCraft& aircraft, Input& input)
{
	return NULL;
}