﻿#include "AirCraft.h"

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

AirCraftState* AirCraftBAmmoState::HandleInput(AirCraft& aircraft, Input& input)
{
	return NULL;
}