#include "GunBossStage1.h"

GunBossStage1State::GunBossStage1State()
{
	time = 0.0f;
}

GunBossStage1State::~GunBossStage1State()
{

};

GunBossStage1State* GunBossStage1State::HandleInput(GunBossStage1&, Input&)
{
	return NULL;
};