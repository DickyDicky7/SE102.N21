#include "Bill.h"

BillState::BillState(DIRECTION direction)
{
	this->time = 0.0f;
	this->direction = direction;
}

BillState::~BillState()
{
}