#include "BossStage3Gate.h"

BossStage3GateCloseState::BossStage3GateCloseState() : BossStage3GateState()
{
}

BossStage3GateCloseState::~BossStage3GateCloseState()
{
}


void BossStage3GateCloseState::Exit(BossStage3Gate& bossStage3Gate)
{
}

void BossStage3GateCloseState::Enter(BossStage3Gate& bossStage3Gate)
{
}

void BossStage3GateCloseState::Render(BossStage3Gate& bossStage3Gate)
{
	bossStage3Gate.SetAnimation(BOSS_STAGE_3_GATE_ANIMATION_ID::CLOSE, bossStage3Gate.GetPosition(), bossStage3Gate.GetMovingDirection(), bossStage3Gate.GetAngle());
}

BossStage3GateState* BossStage3GateCloseState::Update(BossStage3Gate& bossStage3Gate)
{
	if (bossStage3Gate.IsDead() || bossStage3Gate.IsHeadDead()) return new BossStage3GateOpenState();
	return nullptr;
}

BossStage3GateState* BossStage3GateCloseState::HandleInput(BossStage3Gate& bossStage3Gate, Input& input)
{
	return nullptr;
}
