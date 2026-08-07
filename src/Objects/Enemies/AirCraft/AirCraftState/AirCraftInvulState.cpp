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

AirCraftState* AirCraftInvulState::HandleInput(AirCraft& aircraft, Input& input)
{
	return NULL;
}