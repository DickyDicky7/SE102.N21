#include "FinalBossStage1.h"

FinalBossStage1DestroyState::FinalBossStage1DestroyState()
{
	isDestroy = false;
};

FinalBossStage1DestroyState::~FinalBossStage1DestroyState()
{

};

void FinalBossStage1DestroyState::Exit(FinalBossStage1&)
{

}

void FinalBossStage1DestroyState::Enter(FinalBossStage1&)
{

}

void FinalBossStage1DestroyState::Render(FinalBossStage1& finalBoss)
{
	if (!isDestroy)
	{
		finalBoss.SetAnimation(EXPLOSION_ANIMATION_ID::TYPE_3, finalBoss.GetPosition(), finalBoss.GetMovingDirection(), finalBoss.GetAngle());
		return;
	}

	finalBoss.SetAnimation(BOSS_STAGE_1_ANIMATION_ID::FINAL_BOSS_DESTROY, finalBoss.GetPosition(), finalBoss.GetMovingDirection(), finalBoss.GetAngle());
}

FinalBossStage1State* FinalBossStage1DestroyState::Update(FinalBossStage1& finalBoss)
{
	if (isDestroy)
	{
		return NULL;
	}

	std::vector<std::tuple<SPRITE_ID, TIME>>& frames = std::get<
		std::vector<std::tuple<SPRITE_ID, TIME>>>(GraphicsDatabase::animations[EXPLOSION_ANIMATION_ID::TYPE_3]);

	if (std::cmp_greater_equal(finalBoss.GetCurrentFrame() + 1, frames.size()))
	{
		finalBoss.SetX(finalBoss.GetX() + 37.4f);
		finalBoss.SetY(finalBoss.GetY() - 8.0f);
		isDestroy = true;
	}
	return NULL;
}