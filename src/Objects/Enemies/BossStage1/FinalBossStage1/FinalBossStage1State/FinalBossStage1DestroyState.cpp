#include "FinalBossStage1.h"

FinalBossStage1DestroyState::FinalBossStage1DestroyState()
{
	this->_isDestroy = false;
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

void FinalBossStage1DestroyState::Render(FinalBossStage1& finalBossStage1)
{
	if (!this->_isDestroy)
	{
		finalBossStage1.SetAnimation(EXPLOSION_ANIMATION_ID::TYPE_3, finalBossStage1.GetPosition(), finalBossStage1.GetMovingDirection(), finalBossStage1.GetAngle());
		return;
	}

	finalBossStage1.SetAnimation(BOSS_STAGE_1_ANIMATION_ID::FINAL_BOSS_DESTROY, finalBossStage1.GetPosition(), finalBossStage1.GetMovingDirection(), finalBossStage1.GetAngle());
}

FinalBossStage1State* FinalBossStage1DestroyState::Update(FinalBossStage1& finalBossStage1)
{
	if (this->_isDestroy)
	{
		return nullptr;
	}

	std::vector<std::tuple<SPRITE_ID, TIME>>& frames = std::get<
		std::vector<std::tuple<SPRITE_ID, TIME>>>(GraphicsDatabase::animations[EXPLOSION_ANIMATION_ID::TYPE_3]);

	if (std::cmp_greater_equal(finalBossStage1.GetCurrentFrame() + 1, frames.size()))
	{
		finalBossStage1.SetX(finalBossStage1.GetX() + Constants::Enemies::BossStage1::FinalBoss::DESTROY_POSITION_OFFSET_X);
		finalBossStage1.SetY(finalBossStage1.GetY() + Constants::Enemies::BossStage1::FinalBoss::DESTROY_POSITION_OFFSET_Y);
		this->_isDestroy = true;
	}
	return nullptr;
}
