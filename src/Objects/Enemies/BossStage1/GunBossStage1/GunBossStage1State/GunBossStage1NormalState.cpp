#include "GunBossStage1.h"

GunBossStage1NormalState::GunBossStage1NormalState()
{

}

GunBossStage1NormalState::GunBossStage1NormalState(float time)
{
	this->_time = time;
}

GunBossStage1NormalState::~GunBossStage1NormalState()
{

}

void GunBossStage1NormalState::Exit(GunBossStage1&)
{

}

void GunBossStage1NormalState::Enter(GunBossStage1&)
{

}

void GunBossStage1NormalState::Render(GunBossStage1& gunBossStage1)
{
	if (gunBossStage1.GetType() == Constants::Enemies::BossStage1::Gun::TYPE_UPPER)
	{
		gunBossStage1.SetAnimation(BOSS_STAGE_1_ANIMATION_ID::GUN_BOSS_01, gunBossStage1.GetPosition(), gunBossStage1.GetMovingDirection(), gunBossStage1.GetAngle());
		return;
	}

	gunBossStage1.SetAnimation(BOSS_STAGE_1_ANIMATION_ID::GUN_BOSS_02, gunBossStage1.GetPosition(), gunBossStage1.GetMovingDirection(), gunBossStage1.GetAngle());
}

GunBossStage1State* GunBossStage1NormalState::Update(GunBossStage1& gunBossStage1)
{
	if (gunBossStage1.IsDead())
	{
		float newX = gunBossStage1.GetX();
		newX = gunBossStage1.GetX() + Constants::Enemies::BossStage1::Gun::STEP_DISTANCE_X;
		gunBossStage1.SetX(newX);
		return new GunBossStage1DestroyState();
	}

	if (GetTickCount64() - this->_time >= Constants::Enemies::BossStage1::Gun::NORMAL_STEP_INTERVAL_MILLISECONDS)
	{
		float newX = gunBossStage1.GetX();
		newX = gunBossStage1.GetX() + Constants::Enemies::BossStage1::Gun::STEP_DISTANCE_X;
		gunBossStage1.SetX(newX);
		return new GunBossStage1PrepareShootState(static_cast<float>(GetTickCount64()));
	}

	return nullptr;
}
