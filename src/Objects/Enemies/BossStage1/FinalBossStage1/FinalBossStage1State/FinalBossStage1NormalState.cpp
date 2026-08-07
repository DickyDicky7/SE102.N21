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

void FinalBossStage1NormalState::Render(FinalBossStage1& finalBoss)
{
	finalBoss.SetAnimation(BOSS_STAGE_1_ANIMATION_ID::FINAL_BOSS_NORMAL, finalBoss.GetPosition(), finalBoss.GetMovingDirection(), finalBoss.GetAngle());
}

FinalBossStage1State* FinalBossStage1NormalState::Update(FinalBossStage1& finalBoss)
{
	if (finalBoss.isDead)
	{
		//finalBoss.SetX(finalBoss.GetX() + 37.4f);
		//finalBoss.SetY(finalBoss.GetY() - 8.0f);

		return new FinalBossStage1DestroyState();
	}
	return NULL;
}