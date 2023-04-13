#include "AirCraft.h"

AirCraftBAmmoState::AirCraftBAmmoState() : AirCraftState()
{
}

AirCraftBAmmoState::~AirCraftBAmmoState()
{
}


void AirCraftBAmmoState::Exit(AirCraft& aircraft)
{
}

void AirCraftBAmmoState::Enter(AirCraft& aircraft)
{
}

void AirCraftBAmmoState::Render(AirCraft& aircraft)
{
	aircraft.SetAnimation(AIRCRAFT_ANIMATION_ID::B_AMMO, aircraft.GetPosition(), aircraft.GetMovingDirection(), aircraft.GetAngle());
}

AirCraftState* AirCraftBAmmoState::Update(AirCraft& aircraft)
{
	return NULL;
}

AirCraftState* AirCraftBAmmoState::HandleInput(AirCraft& aircraft, Input& input)
{
	return NULL;
}