#include "GunBossStage1.h"

GunBossStage1State::GunBossStage1State()
{
	this->_time = 0.0f;
}

GunBossStage1State::~GunBossStage1State()
{

};

GunBossStage1State* GunBossStage1State::HandleInput(GunBossStage1&, Input&)
{
	return nullptr;
};
