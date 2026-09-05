#include "BossStage3.h"

BossStage3State::BossStage3State()
{
	this->_time = Constants::Enemies::BossStage3::Head::OPEN_DURATION_FRAMES;
}

BossStage3State::~BossStage3State()
{
	this->_time = 0;
}

