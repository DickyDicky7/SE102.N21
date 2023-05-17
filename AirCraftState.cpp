#include "AirCraft.h"

AirCraftState::AirCraftState() 
{
	this->time = 0.0f;
	y0 = 200.0f;
	dt = 0.03f;
	T = 5.0f;
	A = 50.0f;
	φ = 0.0f;
}

AirCraftState::~AirCraftState()
{
}