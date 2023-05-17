#include "AirCraft.h"

AirCraftState::AirCraftState() 
{
	time = 0.0f;
	y0 = 200.0f;
	dt = 0.03f;
	T = 5.0f;
	A = 50.0f;
	φ = 0.0f;

	v0 = 7.0f;
	θ = 80.0f;
}

AirCraftState::~AirCraftState()
{
}