#include "Falcon.h"

FalconCloseState::FalconCloseState() : FalconState()
{
}

FalconCloseState::~FalconCloseState()
{
}


void FalconCloseState::Exit(Falcon& falcon)
{
}

void FalconCloseState::Enter(Falcon& falcon)
{
}

void FalconCloseState::Render(Falcon& falcon)
{
	falcon.SetAnimation(FALCON_ANIMATION_ID::CLOSE, falcon.GetPosition(), falcon.GetMovingDirection(), falcon.GetAngle());
}

FalconState* FalconCloseState::Update(Falcon& falcon)
{
	return NULL;
}

FalconState* FalconCloseState::HandleInput(Falcon& falcon, Input& input)
{
	/*if (input.IsKey(DIK_O))
	{
		return new FalconOpeningState(FALCON_ANIMATION_ID::OPEN);
	}*/

	return NULL;
}