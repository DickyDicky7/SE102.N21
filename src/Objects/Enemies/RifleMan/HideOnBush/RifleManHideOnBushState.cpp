#include "RifleManHideOnBush.h"

RifleManHideOnBushState::RifleManHideOnBushState()
{
	this->_time = GetTickCount64();
}

RifleManHideOnBushState::~RifleManHideOnBushState()
{

}

RifleManHideOnBushState* RifleManHideOnBushState::HandleInput(RifleManHideOnBush& rifleManHideOnBush, Input& input)
{
	return nullptr;
}

bool RifleManHideOnBushState::IsHidden() const
{
	return false;
}
