#include "AirCraft.h"

AirCraftInvulState::AirCraftInvulState() : AirCraftState()
{
}

AirCraftInvulState::~AirCraftInvulState()
{
}


void AirCraftInvulState::Exit(AirCraft& aircraft)
{
}

void AirCraftInvulState::Enter(AirCraft& aircraft)
{
}

void AirCraftInvulState::Render(AirCraft& aircraft)
{
	aircraft.SetAnimation(AIRCRAFT_ANIMATION_ID::INVUL, aircraft.GetPosition(), aircraft.GetMovingDirection(), aircraft.GetAngle());
}

AirCraftState* AirCraftInvulState::Update(AirCraft& aircraft)
{
	return NULL;
}

AirCraftState* AirCraftInvulState::HandleInput(AirCraft& aircraft, Input& input)
{
	return NULL;
}