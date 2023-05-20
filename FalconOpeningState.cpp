#include "Falcon.h"

FalconOpeningState::FalconOpeningState(FALCON_ANIMATION_ID nextState) : FalconState()
{
	delayTime = 30;
	this->nextState = nextState;
}

FalconOpeningState::~FalconOpeningState()
{
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
	if (--delayTime == 0) {
		if (nextState == FALCON_ANIMATION_ID::CLOSE) {
			falcon.SetCurrentState(FALCON_ANIMATION_ID::CLOSE);
			return new FalconCloseState();
		}
		else if (nextState == FALCON_ANIMATION_ID::OPEN) {
			falcon.SetCurrentState(FALCON_ANIMATION_ID::OPEN);
			return new FalconOpenState();
		}
	}
	return NULL;
}

FalconState* FalconOpeningState::HandleInput(Falcon& falcon, Input& input)
{
	//invul, b, f, l, m, r, s
	/*if (input.IsKey(DIK_1))
	{
		return new AirCraftInvulState();
	}
	else if (input.IsKey(DIK_2))
	{
		return new AirCraftBAmmoState();
	}
	else if (input.IsKey(DIK_3))
	{
		return new AirCraftFAmmoState();
	}
	else if (input.IsKey(DIK_4))
	{
		return new AirCraftLAmmoState();
	}
	else if (input.IsKey(DIK_5))
	{
		return new AirCraftMAmmoState();
	}
	else if (input.IsKey(DIK_6))
	{
		return new AirCraftRAmmoState();
	}
	else if (input.IsKey(DIK_7))
	{
		return new AirCraftSAmmoState();
	}*/
	return NULL;
}