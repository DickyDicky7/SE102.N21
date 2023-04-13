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
	return NULL;
}

AirCraftState* AirCraftSAmmoState::HandleInput(AirCraft& aircraft, Input& input)
{
	return NULL;
}