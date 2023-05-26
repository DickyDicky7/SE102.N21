#include "AirCraft.h"

AirCraftState::AirCraftState() 
{
	// y0: vị trí lúc đầu
	// time: thời gian
	// dt: delta time ( t = t + dt )
	// T: khoảng thời gian để quay hết 1 vòng (tính bằng giây)
	// A: Bán kính
	// φ: pha ban đầu của dao động (-π<φ<π)
	// v0: vận tốc ném viên đạn
	// θ: góc ném

	x0 = NULL;
	y0 = NULL;
	time = 0.0f;
	dt = 0.03f;
	T = 2.5f;
	A = 35.0f;
	φ = 0.0f;

	v0 = 7.0f;
	θ = 80.0f;
}

AirCraftState::~AirCraftState()
{
	x0 = NULL;
	y0 = NULL;
	time = NULL;
	dt = NULL;
	T = NULL;
	A = NULL;
	φ = NULL;

	v0 = NULL;
	θ = NULL;
}