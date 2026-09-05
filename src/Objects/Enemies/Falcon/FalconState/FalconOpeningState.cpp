#include "Falcon.h"

FalconOpeningState::FalconOpeningState(FALCON_ANIMATION_ID nextState) : FalconState()
{
	this->_delayTime = Constants::Enemies::Falcon::OPENING_DELAY_FRAMES;
	this->_nextState = nextState;
}

FalconOpeningState::~FalconOpeningState()
{
	this->_delayTime = 0;
}


void FalconOpeningState::Exit(Falcon& falcon)
{
}

void FalconOpeningState::Enter(Falcon& falcon)
{
}

void FalconOpeningState::Render(Falcon& falcon)
{
	falcon.SetAnimation(FALCON_ANIMATION_ID::OPENING, falcon.GetPosition(), falcon.GetMovingDirection(), falcon.GetAngle());
}

FalconState* FalconOpeningState::Update(Falcon& falcon)
{
	if (--this->_delayTime == 0) {
		if (this->_nextState == FALCON_ANIMATION_ID::CLOSE) {
			falcon.SetCurrentState(FALCON_ANIMATION_ID::CLOSE);
			return new FalconCloseState();
		}
		else if (this->_nextState == FALCON_ANIMATION_ID::OPEN) {
			falcon.SetCurrentState(FALCON_ANIMATION_ID::OPEN);
			return new FalconOpenState();
		}
	}
	return nullptr;
}

FalconState* FalconOpeningState::HandleInput(Falcon& falcon, Input& input)
{
	return nullptr;
}
