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
	return NULL;
}

AirCraftState* AirCraftMAmmoState::HandleInput(AirCraft& aircraft, Input& input)
{
	return NULL;
}