#include "Bridge.h"

BridgePart::BridgePart()
{
	this->vx = 1.0f;
	this->vy = 1.0f;
	this->ax = 0.1f;
	this->ay = 0.1f;

	this->name = L"BridgePart\n";

	state = new BridgePartState(BRIDGE_ANIMATION_ID::BODY);
	updateState = NULL;
	isDestroy = false;
}

BridgePart::BridgePart(ANIMATION_ID animationId)
{
	this->vx = 1.0f;
	this->vy = 1.0f;
	this->ax = 0.1f;
	this->ay = 0.1f;

	this->name = L"BridgePart\n";

	state = new BridgePartState(animationId);
	updateState = NULL;
	isDestroy = false;
}

BridgePart::~BridgePart()
{

}

void BridgePart::Update()
{
	updateState = state->Update(*this);
}

void BridgePart::Render()
{
	if (!isDestroy)
	{
		state->Render(*this);
		
		if (dynamic_cast<BridgePartExplosionState*>(state))
		{
			this->w = 0.0f;
			this->h = 0.0f;
		}
		else
		{
			this->w = this->currentFrameW;
			this->h = this->currentFrameH;
		}

		if (updateState)
		{
			ChangeState(state, updateState, this);
			updateState = NULL;
		}
	}
}

void BridgePart::HandleInput(Input&)
{

}

void BridgePart::LoadAnimations()
{

}

void BridgePart::SetIsDestroy(BOOLEAN value)
{
	isDestroy = value;
}

BOOLEAN BridgePart::GetIsDestroy()
{
	return isDestroy;
}