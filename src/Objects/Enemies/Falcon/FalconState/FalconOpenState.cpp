#include "Falcon.h"

FalconOpenState::FalconOpenState() : FalconState()
{
}

FalconOpenState::~FalconOpenState()
{
}


void FalconOpenState::Exit(Falcon& falcon)
{
}

void FalconOpenState::Enter(Falcon& falcon)
{
}

void FalconOpenState::Render(Falcon& falcon)
{
	falcon.SetAnimation(FALCON_ANIMATION_ID::OPEN, falcon.GetPosition(), falcon.GetMovingDirection(), falcon.GetAngle());
}

FalconState* FalconOpenState::Update(Falcon& falcon)
{
	return NULL;
}

FalconState* FalconOpenState::HandleInput(Falcon& falcon, Input& input)
{
	/*if (input.IsKey(DIK_P))
	{
		return new FalconOpeningState(FALCON_ANIMATION_ID::CLOSE);
	}*/

	return NULL;
}