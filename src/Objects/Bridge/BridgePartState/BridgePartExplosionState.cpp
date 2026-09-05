#include "Bridge.h"

BridgePartExplosionState::BridgePartExplosionState()
{

}

BridgePartExplosionState::~BridgePartExplosionState()
{

}

void BridgePartExplosionState::Exit(BridgePart&)
{

}

void BridgePartExplosionState::Enter(BridgePart&)
{

}

void BridgePartExplosionState::Render(BridgePart& bridgePart)
{
	bridgePart.SetAnimation(BRIDGE_ANIMATION_ID::EXPLOSION, bridgePart.GetPosition(), bridgePart.GetMovingDirection(), bridgePart.GetAngle());
}

BridgePartState* BridgePartExplosionState::Update(BridgePart& bridgePart)
{
	std::vector<std::tuple<SPRITE_ID, TIME>>& frames = std::get<
		std::vector<std::tuple<SPRITE_ID, TIME>>>(GraphicsDatabase::animations[BRIDGE_ANIMATION_ID::EXPLOSION]);

	if (std::cmp_greater_equal(bridgePart.GetCurrentFrame() + 1, frames.size()))
	{
		this->_time = static_cast<float>(GetTickCount64());
		bridgePart.SetIsDestroy(true);

		return nullptr;
	}

	return nullptr;
}
