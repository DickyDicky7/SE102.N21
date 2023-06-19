#include "GunBossStage1.h"

GunBossStage1DestroyState::GunBossStage1DestroyState()
{
	isDestroy = false;
	isInDestroyPos = false;
}

GunBossStage1DestroyState::~GunBossStage1DestroyState()
{

}

void GunBossStage1DestroyState::Exit(GunBossStage1&)
{

}

void GunBossStage1DestroyState::Enter(GunBossStage1&)
{

}

void GunBossStage1DestroyState::Render(GunBossStage1& gunBoss)
{
	if (!isDestroy)
	{
		gunBoss.SetAnimation(EXPLOSION_ANIMATION_ID::TYPE_3, gunBoss.GetPosition(), gunBoss.GetMovingDirection(), gunBoss.GetAngle());
		return;
	}


	if (gunBoss.GetType() == 1)
	{
		gunBoss.SetAnimation(BOSS_STAGE_1_ANIMATION_ID::GUN_BOSS_DESTROY_01, gunBoss.GetPosition(), gunBoss.GetMovingDirection(), gunBoss.GetAngle());
		return;
	}

	gunBoss.SetAnimation(BOSS_STAGE_1_ANIMATION_ID::GUN_BOSS_DESTROY_02, gunBoss.GetPosition(), gunBoss.GetMovingDirection(), gunBoss.GetAngle());
}

GunBossStage1State* GunBossStage1DestroyState::Update(GunBossStage1& gunBoss)
{
	if (isDestroy)
	{
		if (!isInDestroyPos)
		{
			float offset = gunBoss.GetType() == 1 ? 8.0f : 3.0f;
			gunBoss.SetX(gunBoss.GetX() + offset);
			gunBoss.SetY(gunBoss.GetY() - 1.0f);
			isInDestroyPos = true;
		}
		return NULL;
	}

	std::vector<std::tuple<SPRITE_ID, TIME>>& frames = std::get<
		std::vector<std::tuple<SPRITE_ID, TIME>>>(GraphicsDatabase::animations[EXPLOSION_ANIMATION_ID::TYPE_3]);

	if (std::cmp_greater_equal(gunBoss.GetCurrentFrame() + 1, frames.size()))
	{
		isDestroy = true;
	}
	
	return NULL;
}