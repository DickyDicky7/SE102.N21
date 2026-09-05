#include "Bridge.h"

BridgePart::BridgePart()
{
	this->_vx = Constants::Physics::DEFAULT_INITIAL_VELOCITY_X;
	this->_vy = Constants::Physics::DEFAULT_INITIAL_VELOCITY_Y;
	this->_ax = Constants::Physics::DEFAULT_INITIAL_ACCELERATION_X;
	this->_ay = Constants::Physics::DEFAULT_INITIAL_ACCELERATION_Y;

	this->SetDebugName(L"BridgePart\n");

	this->_state = new BridgePartState(BRIDGE_ANIMATION_ID::BODY);
	this->_updateState = nullptr;
	this->_isDestroy = false;
}

BridgePart::BridgePart(ANIMATION_ID animationId)
{
	this->_vx = Constants::Physics::DEFAULT_INITIAL_VELOCITY_X;
	this->_vy = Constants::Physics::DEFAULT_INITIAL_VELOCITY_Y;
	this->_ax = Constants::Physics::DEFAULT_INITIAL_ACCELERATION_X;
	this->_ay = Constants::Physics::DEFAULT_INITIAL_ACCELERATION_Y;

	this->SetDebugName(L"BridgePart\n");

	this->_state = new BridgePartState(animationId);
	this->_updateState = nullptr;
	this->_isDestroy = false;
}

BridgePart::~BridgePart()
{
	Destroy(this->_state);
	Destroy(this->_updateState);
}

void BridgePart::Update()
{
	DeferState(this->_updateState, this->_state->Update(*this));

	// Unconditional, where Render applied this from inside its `!_isDestroy`
	// guard: that guard is about whether the part is still drawn, not about
	// whether its state machine may advance.
	ApplyDeferredState(this->_state, this->_updateState, this);
}

void BridgePart::Render()
{
	if (!this->_isDestroy)
	{
		this->_state->Render(*this);

		if (this->_state && this->_state->HasZeroDimensions())
		{
			this->_w = 0.0f;
			this->_h = 0.0f;
		}
		else
		{
			this->_w = this->GetCurrentFrameW();
			this->_h = this->GetCurrentFrameH();
		}
	}
}

void BridgePart::HandleInput(Input&)
{

}

void BridgePart::LoadAnimations()
{

}

void BridgePart::SetIsDestroy(bool isDestroy)
{
	this->_isDestroy = isDestroy;
}

bool BridgePart::GetIsDestroy() const
{
	return this->_isDestroy;
}
