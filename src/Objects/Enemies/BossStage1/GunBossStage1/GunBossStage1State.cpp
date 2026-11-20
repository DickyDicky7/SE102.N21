#include "GunBossStage1.h"

GunBossStage1State::GunBossStage1State()
{
	this->_time = GetTickCount64();
}

GunBossStage1State::~GunBossStage1State()
{

};

GunBossStage1State* GunBossStage1State::HandleInput(GunBossStage1&, Input&)
{
	return nullptr;
};
