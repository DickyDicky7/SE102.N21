#include "Bridge.h"

BridgePartState::BridgePartState()
{
	this->time = 0.0f;
	animationId = BRIDGE_ANIMATION_ID::BODY;
}

BridgePartState::BridgePartState(ANIMATION_ID _animationId)
{
	this->time = 0.0f;
	animationId = _animationId;
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
	bridgePart.SetAnimation(this->animationId, bridgePart.GetPosition(), bridgePart.GetMovingDirection(), bridgePart.GetAngle());
}

BridgePartState* BridgePartState::Update(BridgePart& bridgePart)
{
	std::vector<std::tuple<SPRITE_ID, TIME>>& frames = std::get<
		std::vector<std::tuple<SPRITE_ID, TIME>>>(GraphicsDatabase::animations[this->animationId]);

	if (std::cmp_greater_equal(bridgePart.GetCurrentFrame() + 1, frames.size()))
	{
		this->time = (FLOAT)GetTickCount64();
		Sound::getInstance()->play("bridgeexplosion.wav", false, 1);
		return new BridgePartExplosionState();
	}
	return NULL;
}

BridgePartState* BridgePartState::HandleInput(BridgePart&, Input&)
{
	return NULL;
}