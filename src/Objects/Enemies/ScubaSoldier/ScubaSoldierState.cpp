#include "ScubaSoldier.h"

ScubaSoldierState::ScubaSoldierState()
{
	this->_time = 0.0f;
}

ScubaSoldierState::~ScubaSoldierState()
{
	this->_time = 0.0f;
}

bool ScubaSoldierState::IsHidden() const
{
	return false;
}
