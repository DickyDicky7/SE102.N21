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
	return NULL;
}

AirCraftState* AirCraftFAmmoState::HandleInput(AirCraft& aircraft, Input& input)
{
	return NULL;
}