#include "BossStage3.h"

BossStage3OpenState::BossStage3OpenState() : BossStage3State()
{
}

BossStage3OpenState::~BossStage3OpenState()
{
}


void BossStage3OpenState::Exit(BossStage3& bossStage3)
{
}

void BossStage3OpenState::Enter(BossStage3& bossStage3)
{
}

void BossStage3OpenState::Render(BossStage3& bossStage3)
{
	bossStage3.SetAnimation(BOSS_STAGE_3_ANIMATION_ID::OPEN, bossStage3.GetPosition(), bossStage3.GetMovingDirection(), bossStage3.GetAngle());
}

BossStage3State* BossStage3OpenState::Update(BossStage3& bossStage3)
{
	if (time-- < 0) {
		return new BossStage3MiddleState(BOSS_STAGE_3_ANIMATION_ID::CLOSE);
	}
	return NULL;
}

BossStage3State* BossStage3OpenState::HandleInput(BossStage3& bossStage3, Input& input)
{
	if (input.IsKey(DIK_P))
	{
		return new BossStage3MiddleState(BOSS_STAGE_3_ANIMATION_ID::CLOSE);
	}

	return NULL;
}