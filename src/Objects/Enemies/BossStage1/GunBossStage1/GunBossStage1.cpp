#include "GunBossStage1.h"

GunBossStage1::GunBossStage1() : HasWeapons(new BulletBossStage1State())
{
	this->_vx = Constants::Physics::DEFAULT_INITIAL_VELOCITY_X;
	this->_vy = Constants::Physics::DEFAULT_INITIAL_VELOCITY_Y;
	this->_ax = Constants::Physics::DEFAULT_INITIAL_ACCELERATION_X;
	this->_ay = Constants::Physics::DEFAULT_INITIAL_ACCELERATION_Y;
	this->_angle = 0;
	this->_position.x = Constants::Enemies::BossStage1::DEFAULT_SPAWN_X;
	this->_position.y = Constants::Enemies::BossStage1::DEFAULT_SPAWN_Y;

	this->_state = nullptr;
	this->_updateState = nullptr;

	//
	this->_type = Constants::Enemies::BossStage1::Gun::TYPE_UPPER;
	this->SetDebugName(L"Gun Boss Stage 1\n");
	//
	this->_hitCounts = Constants::Enemies::BossStage1::Gun::HEALTH_POINTS;
	this->_enemyType = ENEMY_TYPE::BOSS;

	this->_firingRate = 0;
}

GunBossStage1::GunBossStage1(int type) : HasWeapons(new BulletBossStage1State())
{
	this->_vx = Constants::Physics::DEFAULT_INITIAL_VELOCITY_X;
	this->_vy = Constants::Physics::DEFAULT_INITIAL_VELOCITY_Y;
	this->_ax = Constants::Physics::DEFAULT_INITIAL_ACCELERATION_X;
	this->_ay = Constants::Physics::DEFAULT_INITIAL_ACCELERATION_Y;
	this->_angle = 0;
	this->_position.x = Constants::Enemies::BossStage1::DEFAULT_SPAWN_X;
	this->_position.y = Constants::Enemies::BossStage1::DEFAULT_SPAWN_Y;

	this->_state = nullptr;
	this->_updateState = nullptr;

	//
	this->_type = type;
	this->SetDebugName(L"Gun Boss Stage 1\n");
	//
	this->_hitCounts = Constants::Enemies::BossStage1::Gun::HEALTH_POINTS;
	this->_enemyType = ENEMY_TYPE::BOSS;

	this->_firingRate = 0;
}

int GunBossStage1::GetType() const
{
	return this->_type;
}

GunBossStage1::~GunBossStage1()
{
	Destroy(this->_state);
	Destroy(this->_updateState);
}

const Bill* GunBossStage1::GetEnemyTarget()
{
	return this->_target;
}

void GunBossStage1::Update()
{
	if (!this->_state)
	{
		this->_state = new GunBossStage1NormalState();
	}

	DeferState(this->_updateState, this->_state->Update(*this));

	ApplyDeferredState(this->_state, this->_updateState, this);
};

void GunBossStage1::Render()
{
	this->_state->Render(*this);
	this->_w = this->GetCurrentFrameW();
	this->_h = this->GetCurrentFrameH();
};

void GunBossStage1::HandleInput(Input&)
{

};

void GunBossStage1::LoadTextures()
{
	BossStage1::LoadTextures();
};

void GunBossStage1::LoadSprites()
{
	if (HasSprites<GunBossStage1>::_hasBeenLoaded) {
		return;
	}
	HasSprites<GunBossStage1>::_hasBeenLoaded = true;

	GraphicsHelper::InsertSprite(BOSS_STAGE_1_SPRITE_ID::GUN_BOSS_01, 42, 27, 40, 48, DIRECTION::LEFT, BOSS_STAGE_1_TEXTURE_ID::BOSS_STAGE_1);
	GraphicsHelper::InsertSprite(BOSS_STAGE_1_SPRITE_ID::GUN_BOSS_01_HALF, 42, 27, 39, 48, DIRECTION::LEFT, BOSS_STAGE_1_TEXTURE_ID::BOSS_STAGE_1);

	GraphicsHelper::InsertSprite(BOSS_STAGE_1_SPRITE_ID::GUN_BOSS_02, 42, 48, 59, 48, DIRECTION::LEFT, BOSS_STAGE_1_TEXTURE_ID::BOSS_STAGE_1);
	GraphicsHelper::InsertSprite(BOSS_STAGE_1_SPRITE_ID::GUN_BOSS_02_HALF, 42, 48, 58, 48, DIRECTION::LEFT, BOSS_STAGE_1_TEXTURE_ID::BOSS_STAGE_1);

	GraphicsHelper::InsertSprite(BOSS_STAGE_1_SPRITE_ID::GUN_BOSS_DESTROY_01, 42, 0, 15, 50, DIRECTION::LEFT, BOSS_STAGE_1_TEXTURE_ID::BOSS_STAGE_1);
	GraphicsHelper::InsertSprite(BOSS_STAGE_1_SPRITE_ID::GUN_BOSS_DESTROY_02, 42, 19, 23, 47, DIRECTION::LEFT, BOSS_STAGE_1_TEXTURE_ID::BOSS_STAGE_1);
};

void GunBossStage1::LoadAnimations()
{
	if (HasAnimations<GunBossStage1>::_hasBeenLoaded) {
		return;
	}
	HasAnimations<GunBossStage1>::_hasBeenLoaded = true;

	GraphicsHelper::InsertAnimation(BOSS_STAGE_1_ANIMATION_ID::GUN_BOSS_01, Constants::Enemies::BossStage1::ANIMATION_DELAY_MILLISECONDS,
		{
			{BOSS_STAGE_1_SPRITE_ID::GUN_BOSS_01, 0},
		});

	GraphicsHelper::InsertAnimation(BOSS_STAGE_1_ANIMATION_ID::GUN_BOSS_01_HALF, Constants::Enemies::BossStage1::ANIMATION_DELAY_MILLISECONDS,
		{
			{BOSS_STAGE_1_SPRITE_ID::GUN_BOSS_01_HALF, 0},
		});

	GraphicsHelper::InsertAnimation(BOSS_STAGE_1_ANIMATION_ID::GUN_BOSS_02, Constants::Enemies::BossStage1::ANIMATION_DELAY_MILLISECONDS,
		{
			{BOSS_STAGE_1_SPRITE_ID::GUN_BOSS_02, 0},
		});

	GraphicsHelper::InsertAnimation(BOSS_STAGE_1_ANIMATION_ID::GUN_BOSS_02_HALF, Constants::Enemies::BossStage1::ANIMATION_DELAY_MILLISECONDS,
		{
			{BOSS_STAGE_1_SPRITE_ID::GUN_BOSS_02_HALF, 0},
		});

	GraphicsHelper::InsertAnimation(BOSS_STAGE_1_ANIMATION_ID::GUN_BOSS_DESTROY_01, Constants::Enemies::BossStage1::ANIMATION_DELAY_MILLISECONDS,
		{
			{BOSS_STAGE_1_SPRITE_ID::GUN_BOSS_DESTROY_01, 0},
		});

	GraphicsHelper::InsertAnimation(BOSS_STAGE_1_ANIMATION_ID::GUN_BOSS_DESTROY_02, Constants::Enemies::BossStage1::ANIMATION_DELAY_MILLISECONDS,
		{
			{BOSS_STAGE_1_SPRITE_ID::GUN_BOSS_DESTROY_02, 0},
		});
};

void GunBossStage1::Fire()
{
	if (!Enemy::_target || Enemy::_target->IsDead())
	{
		return;
	}
}

void GunBossStage1::Fire(float angle, float vx, float vy, float ax, float ay, DIRECTION direction)
{
	if (!Enemy::_target || Enemy::_target->IsDead())
	{
		return;
	}
	HasWeapons::Fire(this->_position.x - this->_w * Constants::Enemies::BossStage1::Gun::MUZZLE_OFFSET_RATIO_X, this->_position.y, angle, vx, vy, ax, ay, direction);
}
