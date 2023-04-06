#include "BossStage3.h"

BossStage3MiddleState::BossStage3MiddleState(BOSS_STAGE_3_ANIMATION_ID nextState) : BossStage3State()
{
	delayTime = 50;
	this->nextState = nextState;
}

BossStage3MiddleState::~BossStage3MiddleState()
{
}


void BossStage3MiddleState::Exit(BossStage3& bossStage3)
{
}

void BossStage3MiddleState::Enter(BossStage3& bossStage3)
{
}

void BossStage3MiddleState::Render(BossStage3& bossStage3)
{
	bossStage3.SetAnimation(BOSS_STAGE_3_ANIMATION_ID::MIDDLE, bossStage3.GetPosition(), bossStage3.GetMovingDirection(), bossStage3.GetAngle());
}

BossStage3State* BossStage3MiddleState::Update(BossStage3& bossStage3)
{
	if (--delayTime == 0) {
		if (nextState == BOSS_STAGE_3_ANIMATION_ID::CLOSE) {
			return new BossStage3CloseState();
		}
		else if (nextState == BOSS_STAGE_3_ANIMATION_ID::OPEN) {
			return new BossStage3OpenState();
		}
	}
	return NULL;
}
BossStage3State* BossStage3MiddleState::HandleInput(BossStage3& bossStage3, Input& input)
{
	return NULL;
}