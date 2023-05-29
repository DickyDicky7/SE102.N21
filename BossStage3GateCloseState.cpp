#include "BossStage3Gate.h"

BossStage3GateCloseState::BossStage3GateCloseState() : BossStage3GateState()
{
}

BossStage3GateCloseState::~BossStage3GateCloseState()
{
}


void BossStage3GateCloseState::Exit(BossStage3Gate& BossStage3Gate)
{
}

void BossStage3GateCloseState::Enter(BossStage3Gate& BossStage3Gate)
{
}

void BossStage3GateCloseState::Render(BossStage3Gate& BossStage3Gate)
{
	BossStage3Gate.SetAnimation(BOSS_STAGE_3_GATE_ANIMATION_ID::CLOSE, BossStage3Gate.GetPosition(), BossStage3Gate.GetMovingDirection(), BossStage3Gate.GetAngle());
}

BossStage3GateState* BossStage3GateCloseState::Update(BossStage3Gate& BossStage3Gate)
{
	return NULL;
}

BossStage3GateState* BossStage3GateCloseState::HandleInput(BossStage3Gate& BossStage3Gate, Input& input)
{
	return NULL;
}