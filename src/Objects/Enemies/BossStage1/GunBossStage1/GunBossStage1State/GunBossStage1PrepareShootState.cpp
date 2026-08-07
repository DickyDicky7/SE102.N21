#include "GunBossStage1.h"

GunBossStage1PrepareShootState::GunBossStage1PrepareShootState()
{

}

GunBossStage1PrepareShootState::GunBossStage1PrepareShootState(FLOAT _time)
{
	time = _time;
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

void GunBossStage1PrepareShootState::Render(GunBossStage1& gunBoss)
{
	if (gunBoss.GetType() == 1)
	{
		gunBoss.SetAnimation(BOSS_STAGE_1_ANIMATION_ID::GUN_BOSS_01_HALF, gunBoss.GetPosition(), gunBoss.GetMovingDirection(), gunBoss.GetAngle());
		return;
	}

	gunBoss.SetAnimation(BOSS_STAGE_1_ANIMATION_ID::GUN_BOSS_02_HALF, gunBoss.GetPosition(), gunBoss.GetMovingDirection(), gunBoss.GetAngle());
}

GunBossStage1State* GunBossStage1PrepareShootState::Update(GunBossStage1& gunBoss)
{
	if (gunBoss.isDead)
	{
		float newX = gunBoss.GetX();
		newX = gunBoss.GetX() - 1.0f;
		gunBoss.SetX(newX);
		return new GunBossStage1DestroyState();
	}

	if (GetTickCount64() - this->time >= 500.0f)
	{
		float vx = abs(gunBoss.GetX() - gunBoss.GetEnemyTarget()->GetX()) / 32.0f;
		gunBoss.Fire(vx, 0.0f, 0.0f, 0.0f, 0.0f, gunBoss.GetMovingDirection());

		float newX = gunBoss.GetX();
		newX = gunBoss.GetX() - 1.0f;
		gunBoss.SetX(newX);
		return new GunBossStage1NormalState((FLOAT)GetTickCount64());
	}

	return NULL;
}