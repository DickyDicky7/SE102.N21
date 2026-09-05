#include "BossStage3Gate.h"

BossStage3GateOpenState::BossStage3GateOpenState() : BossStage3GateState()
{
}

BossStage3GateOpenState::~BossStage3GateOpenState()
{
}


void BossStage3GateOpenState::Exit(BossStage3Gate& bossStage3Gate)
{
}

void BossStage3GateOpenState::Enter(BossStage3Gate& bossStage3Gate)
{
}

void BossStage3GateOpenState::Render(BossStage3Gate& bossStage3Gate)
{
	bossStage3Gate.SetAnimation(BOSS_STAGE_3_GATE_ANIMATION_ID::OPEN, bossStage3Gate.GetPosition(), bossStage3Gate.GetMovingDirection(), bossStage3Gate.GetAngle());
}

BossStage3GateState* BossStage3GateOpenState::Update(BossStage3Gate& bossStage3Gate)
{
	return nullptr;
}

BossStage3GateState* BossStage3GateOpenState::HandleInput(BossStage3Gate& bossStage3Gate, Input& input)
{
	return nullptr;
}
