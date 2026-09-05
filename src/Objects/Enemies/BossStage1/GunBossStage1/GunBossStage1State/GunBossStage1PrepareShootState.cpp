#include "GunBossStage1.h"

GunBossStage1PrepareShootState::GunBossStage1PrepareShootState()
{

}

GunBossStage1PrepareShootState::GunBossStage1PrepareShootState(float time)
{
	this->_time = time;
}

GunBossStage1PrepareShootState::~GunBossStage1PrepareShootState()
{

}

void GunBossStage1PrepareShootState::Exit(GunBossStage1&)
{

}

void GunBossStage1PrepareShootState::Enter(GunBossStage1&)
{

}

void GunBossStage1PrepareShootState::Render(GunBossStage1& gunBossStage1)
{
	if (gunBossStage1.GetType() == Constants::Enemies::BossStage1::Gun::TYPE_UPPER)
	{
		gunBossStage1.SetAnimation(BOSS_STAGE_1_ANIMATION_ID::GUN_BOSS_01_HALF, gunBossStage1.GetPosition(), gunBossStage1.GetMovingDirection(), gunBossStage1.GetAngle());
		return;
	}

	gunBossStage1.SetAnimation(BOSS_STAGE_1_ANIMATION_ID::GUN_BOSS_02_HALF, gunBossStage1.GetPosition(), gunBossStage1.GetMovingDirection(), gunBossStage1.GetAngle());
}

GunBossStage1State* GunBossStage1PrepareShootState::Update(GunBossStage1& gunBossStage1)
{
	if (gunBossStage1.IsDead())
	{
		float newX = gunBossStage1.GetX();
		newX = gunBossStage1.GetX() - Constants::Enemies::BossStage1::Gun::STEP_DISTANCE_X;
		gunBossStage1.SetX(newX);
		return new GunBossStage1DestroyState();
	}

	if (GetTickCount64() - this->_time >= Constants::Enemies::BossStage1::Gun::PREPARE_SHOOT_INTERVAL_MILLISECONDS)
	{
		const Bill* target = gunBossStage1.GetEnemyTarget();
		float targetX = target ? target->GetX() : gunBossStage1.GetX();
		float vx = std::abs(gunBossStage1.GetX() - targetX) / Constants::Enemies::BossStage1::Gun::SHOT_SPEED_DISTANCE_DIVISOR;
		gunBossStage1.Fire(vx, 0.0f, 0.0f, 0.0f, 0.0f, gunBossStage1.GetMovingDirection());

		float newX = gunBossStage1.GetX();
		newX = gunBossStage1.GetX() - Constants::Enemies::BossStage1::Gun::STEP_DISTANCE_X;
		gunBossStage1.SetX(newX);
		return new GunBossStage1NormalState(static_cast<float>(GetTickCount64()));
	}

	return nullptr;
}
