#include "RifleManHideOnBush.h"

RifleManHideOnBushState::RifleManHideOnBushState()
{
	this->_time = static_cast<float>(GetTickCount64());
}

RifleManHideOnBushState::~RifleManHideOnBushState()
{

}

RifleManHideOnBushState* RifleManHideOnBushState::HandleInput(RifleManHideOnBush& rifleManHideOnBush, Input& input)
{
	return nullptr;
}
