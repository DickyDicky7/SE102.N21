#include "ScubaSoldier.h"

ScubaSoldierState::ScubaSoldierState()
{
	this->_time = 0;
}

ScubaSoldierState::~ScubaSoldierState()
{
	this->_time = 0;
}

bool ScubaSoldierState::IsHidden() const
{
	return false;
}
