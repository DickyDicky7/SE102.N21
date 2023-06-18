#include "BossStage3.h"

BossStage3CloseState::BossStage3CloseState() : BossStage3State()
{
}

BossStage3CloseState::~BossStage3CloseState()
{
}


void BossStage3CloseState::Exit(BossStage3& bossStage3)
{
}

void BossStage3CloseState::Enter(BossStage3& bossStage3)
{
	bossStage3.SetIsFire(false);
}

void BossStage3CloseState::Render(BossStage3& bossStage3)
{
	bossStage3.SetAnimation(BOSS_STAGE_3_ANIMATION_ID::CLOSE, bossStage3.GetPosition(), bossStage3.GetMovingDirection(), bossStage3.GetAngle());
}

BossStage3State* BossStage3CloseState::Update(BossStage3& bossStage3)
{
	if (time-- < 0) {
		return new BossStage3MiddleState(BOSS_STAGE_3_ANIMATION_ID::OPEN);
	}
	return NULL;
}

BossStage3State* BossStage3CloseState::HandleInput(BossStage3& bossStage3, Input& input)
{
	if (input.IsKey(DIK_O))
	{
		return new BossStage3MiddleState(BOSS_STAGE_3_ANIMATION_ID::OPEN);
	}

	return NULL;
}