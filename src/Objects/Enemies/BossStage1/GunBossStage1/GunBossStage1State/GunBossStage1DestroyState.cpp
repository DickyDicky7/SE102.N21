#include "GunBossStage1.h"

GunBossStage1DestroyState::GunBossStage1DestroyState()
{
	this->_isDestroy = false;
	this->_isInDestroyPos = false;
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

void GunBossStage1DestroyState::Render(GunBossStage1& gunBossStage1)
{
	if (!this->_isDestroy)
	{
		gunBossStage1.SetAnimation(EXPLOSION_ANIMATION_ID::TYPE_3, gunBossStage1.GetPosition(), gunBossStage1.GetMovingDirection(), gunBossStage1.GetAngle());
		return;
	}


	if (gunBossStage1.GetType() == Constants::Enemies::BossStage1::Gun::TYPE_UPPER)
	{
		gunBossStage1.SetAnimation(BOSS_STAGE_1_ANIMATION_ID::GUN_BOSS_DESTROY_01, gunBossStage1.GetPosition(), gunBossStage1.GetMovingDirection(), gunBossStage1.GetAngle());
		return;
	}

	gunBossStage1.SetAnimation(BOSS_STAGE_1_ANIMATION_ID::GUN_BOSS_DESTROY_02, gunBossStage1.GetPosition(), gunBossStage1.GetMovingDirection(), gunBossStage1.GetAngle());
}

GunBossStage1State* GunBossStage1DestroyState::Update(GunBossStage1& gunBossStage1)
{
	if (this->_isDestroy)
	{
		if (!this->_isInDestroyPos)
		{
			float offsetX = gunBossStage1.GetType() == Constants::Enemies::BossStage1::Gun::TYPE_UPPER ? Constants::Enemies::BossStage1::Gun::DESTROY_OFFSET_X_UPPER : Constants::Enemies::BossStage1::Gun::DESTROY_OFFSET_X_LOWER;
			float offsetY = gunBossStage1.GetType() == Constants::Enemies::BossStage1::Gun::TYPE_UPPER ? Constants::Enemies::BossStage1::Gun::DESTROY_OFFSET_Y_UPPER : Constants::Enemies::BossStage1::Gun::DESTROY_OFFSET_Y_LOWER;
			gunBossStage1.SetX(gunBossStage1.GetX() + offsetX);
			gunBossStage1.SetY(gunBossStage1.GetY() - offsetY);
			this->_isInDestroyPos = true;
		}
		return nullptr;
	}

	std::vector<std::tuple<SPRITE_ID, TIME>>& frames = std::get<
		std::vector<std::tuple<SPRITE_ID, TIME>>>(GraphicsDatabase::animations[EXPLOSION_ANIMATION_ID::TYPE_3]);

	if (std::cmp_greater_equal(gunBossStage1.GetCurrentFrame() + 1, frames.size()))
	{
		this->_isDestroy = true;
	}

	return nullptr;
}
