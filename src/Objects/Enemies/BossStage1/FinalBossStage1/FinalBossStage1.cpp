#include "FinalBossStage1.h"

FinalBossStage1::FinalBossStage1()
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

	// Filled in by SetGun1/SetGun2, which the caller is free to skip - and Update
	// reads both unconditionally, so leaving them indeterminate made that read
	// undefined rather than merely wrong.
	this->_gun1 = nullptr;
	this->_gun2 = nullptr;

	this->SetDebugName(L"Final Boss Stage 1\n");

	//
	this->_hitCounts = 0;
	this->_enemyType = ENEMY_TYPE::BOSS;

	this->_deadTurns = 0;
}

FinalBossStage1::~FinalBossStage1()
{
	Destroy(this->_state);
	Destroy(this->_updateState);
}

void FinalBossStage1::Update()
{
	if (this->_gun1 && this->_gun2 && this->_gun1->IsDead() && this->_gun2->IsDead() && this->_hitCounts <= 0 && !this->IsDead())
	{
		this->_hitCounts = Constants::Enemies::BossStage1::FinalBoss::HEALTH_POINTS;
	}

	if (!this->_state)
	{
		this->_state = new FinalBossStage1NormalState();
	}

	DeferState(this->_updateState, this->_state->Update(*this));

	ApplyDeferredState(this->_state, this->_updateState, this);
};

void FinalBossStage1::Render()
{
	// Unlike every other entity, this one starts with a null _state and only
	// creates it lazily in Update - so Render is reachable with none, e.g. via
	// Stage1::RenderBossCompletion.
	if (this->_state)
	{
		this->_state->Render(*this);
	}
	this->_w = this->GetCurrentFrameW();
	this->_h = this->GetCurrentFrameH();
};

void FinalBossStage1::SetGun1(GunBossStage1* gun1)
{
	this->_gun1 = gun1;
}

void FinalBossStage1::SetGun2(GunBossStage1* gun2)
{
	this->_gun2 = gun2;
}

void FinalBossStage1::HandleInput(Input&)
{

};

void FinalBossStage1::LoadTextures()
{
	BossStage1::LoadTextures();
};

void FinalBossStage1::LoadSprites()
{
	if (HasSprites::_hasBeenLoaded) {
		return;
	}
	HasSprites::_hasBeenLoaded = true;

	GraphicsHelper::InsertSprite(BOSS_STAGE_1_SPRITE_ID::FINAL_BOSS_01, 54, 0, 23, 84, DIRECTION::LEFT, BOSS_STAGE_1_TEXTURE_ID::BOSS_STAGE_1);
	GraphicsHelper::InsertSprite(BOSS_STAGE_1_SPRITE_ID::FINAL_BOSS_02, 54, 27, 50, 84, DIRECTION::LEFT, BOSS_STAGE_1_TEXTURE_ID::BOSS_STAGE_1);
	GraphicsHelper::InsertSprite(BOSS_STAGE_1_SPRITE_ID::FINAL_BOSS_03, 54, 54, 77, 84, DIRECTION::LEFT, BOSS_STAGE_1_TEXTURE_ID::BOSS_STAGE_1);

	GraphicsHelper::InsertSprite(BOSS_STAGE_1_SPRITE_ID::FINAL_BOSS_DESTROY, 0, 0, 108, 38, DIRECTION::LEFT, BOSS_STAGE_1_TEXTURE_ID::BOSS_STAGE_1);
};

void FinalBossStage1::LoadAnimations()
{
	if (HasAnimations::_hasBeenLoaded) {
		return;
	}
	HasAnimations::_hasBeenLoaded = true;

	GraphicsHelper::InsertAnimation(BOSS_STAGE_1_ANIMATION_ID::FINAL_BOSS_NORMAL, Constants::Enemies::BossStage1::ANIMATION_DELAY_MILLISECONDS,
		{
			{BOSS_STAGE_1_SPRITE_ID::FINAL_BOSS_01, 0},
			{BOSS_STAGE_1_SPRITE_ID::FINAL_BOSS_02, 0},
			{BOSS_STAGE_1_SPRITE_ID::FINAL_BOSS_03, 0},
		});

	GraphicsHelper::InsertAnimation(BOSS_STAGE_1_ANIMATION_ID::FINAL_BOSS_DESTROY, Constants::Enemies::BossStage1::ANIMATION_DELAY_MILLISECONDS,
		{
			{BOSS_STAGE_1_SPRITE_ID::FINAL_BOSS_DESTROY, 0},
		});
};
