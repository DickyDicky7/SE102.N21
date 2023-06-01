#include "BossStage3Gate.h"

BossStage3GateOpenState::BossStage3GateOpenState() : BossStage3GateState()
{
}

BossStage3GateOpenState::~BossStage3GateOpenState()
{
}


void BossStage3GateOpenState::Exit(BossStage3Gate& BossStage3Gate)
{
}

void BossStage3GateOpenState::Enter(BossStage3Gate& BossStage3Gate)
{
}

void BossStage3GateOpenState::Render(BossStage3Gate& BossStage3Gate)
{
	BossStage3Gate.SetAnimation(BOSS_STAGE_3_GATE_ANIMATION_ID::OPEN, BossStage3Gate.GetPosition(), BossStage3Gate.GetMovingDirection(), BossStage3Gate.GetAngle());
}

BossStage3GateState* BossStage3GateOpenState::Update(BossStage3Gate& BossStage3Gate)
{
	return NULL;
}

BossStage3GateState* BossStage3GateOpenState::HandleInput(BossStage3Gate& BossStage3Gate, Input& input)
{
	return NULL;
}