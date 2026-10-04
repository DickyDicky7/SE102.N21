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

void BridgePartExplosionState::Enter(BridgePart& bridgePart)
{
	// Update ends the part once the explosion reaches its last frame; start it
	// from frame 0 rather than wherever the body animation was.
	bridgePart.ResetAnimationFrame();
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

bool BridgePartExplosionState::HasZeroDimensions() const
{
	return true;
}
