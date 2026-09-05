#include "FinalBossStage1.h"

FinalBossStage1NormalState::FinalBossStage1NormalState()
{

};

FinalBossStage1NormalState::~FinalBossStage1NormalState()
{

};

void FinalBossStage1NormalState::Exit(FinalBossStage1&)
{

}

void FinalBossStage1NormalState::Enter(FinalBossStage1&)
{

}

void FinalBossStage1NormalState::Render(FinalBossStage1& finalBossStage1)
{
	finalBossStage1.SetAnimation(BOSS_STAGE_1_ANIMATION_ID::FINAL_BOSS_NORMAL, finalBossStage1.GetPosition(), finalBossStage1.GetMovingDirection(), finalBossStage1.GetAngle());
}

FinalBossStage1State* FinalBossStage1NormalState::Update(FinalBossStage1& finalBossStage1)
{
	if (finalBossStage1.IsDead())
	{
		return new FinalBossStage1DestroyState();
	}
	return nullptr;
}
