#include "Bridge.h"

BridgePartState::BridgePartState()
{
	this->_time = 0.0f;
	this->_animationId = BRIDGE_ANIMATION_ID::BODY;
}

BridgePartState::BridgePartState(ANIMATION_ID animationId)
{
	this->_time = 0.0f;
	this->_animationId = animationId;
}

BridgePartState::~BridgePartState()
{

}

void BridgePartState::Exit(BridgePart&)
{

}

void BridgePartState::Enter(BridgePart&)
{

}

void BridgePartState::Render(BridgePart& bridgePart)
{
	bridgePart.SetAnimation(this->_animationId, bridgePart.GetPosition(), bridgePart.GetMovingDirection(), bridgePart.GetAngle());
}

BridgePartState* BridgePartState::Update(BridgePart& bridgePart)
{
	std::vector<std::tuple<SPRITE_ID, TIME>>& frames = std::get<
		std::vector<std::tuple<SPRITE_ID, TIME>>>(GraphicsDatabase::animations[this->_animationId]);

	if (std::cmp_greater_equal(bridgePart.GetCurrentFrame() + 1, frames.size()))
	{
		this->_time = static_cast<float>(GetTickCount64());
		Sound::GetInstance()->Play("bridgeexplosion", false, 1);
		return new BridgePartExplosionState();
	}
	return nullptr;
}

BridgePartState* BridgePartState::HandleInput(BridgePart&, Input&)
{
	return nullptr;
}
