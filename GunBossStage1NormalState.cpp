#include "GunBossStage1.h"

GunBossStage1NormalState::GunBossStage1NormalState()
{

}

GunBossStage1NormalState::GunBossStage1NormalState(FLOAT _time)
{
	time = _time;
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

void GunBossStage1NormalState::Render(GunBossStage1& gunBoss)
{
	if (gunBoss.GetType() == 1)
	{
		gunBoss.SetAnimation(BOSS_STAGE_1_ANIMATION_ID::GUN_BOSS_01, gunBoss.GetPosition(), gunBoss.GetMovingDirection(), gunBoss.GetAngle());
		return;
	}

	gunBoss.SetAnimation(BOSS_STAGE_1_ANIMATION_ID::GUN_BOSS_02, gunBoss.GetPosition(), gunBoss.GetMovingDirection(), gunBoss.GetAngle());
}

GunBossStage1State* GunBossStage1NormalState::Update(GunBossStage1& gunBoss)
{
	if (gunBoss.isDead)
	{
		float newX = gunBoss.GetX();
		newX = gunBoss.GetX() + 1.0f;
		gunBoss.SetX(newX);
		return new GunBossStage1DestroyState();
	}

	if (GetTickCount64() - this->time >= 400.0f)
	{
		float newX = gunBoss.GetX();
		newX = gunBoss.GetX() + 1.0f;
		gunBoss.SetX(newX);
		return new GunBossStage1PrepareShootState((FLOAT)GetTickCount64());
	}

	return NULL;
}