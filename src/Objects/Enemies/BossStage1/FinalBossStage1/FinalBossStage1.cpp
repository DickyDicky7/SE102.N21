#include "FinalBossStage1.h"

FinalBossStage1::FinalBossStage1()
{
	this->vx = 1.0f;
	this->vy = 1.0f;
	this->ax = 0.1f;
	this->ay = 0.1f;
	this->angle = 0;
	this->position.x = 50;
	this->position.y = 00;

	this->state = NULL;
	this->updateState = NULL;

	this->name = L"Final Boss Stage 1\n";

	//
	this->hitCounts = 0;
	this->enemyType = ENEMY_TYPE::BOSS;

	this->deadTurns = 0;
}

FinalBossStage1::~FinalBossStage1()
{

};

void FinalBossStage1::Update()
{
	if (gun1->isDead && gun2->isDead && hitCounts <= 0 && !isDead)
	{
		hitCounts = 32;
	}

	if (!state)
	{
		state = new FinalBossStage1NormalState();
	}

	updateState = state->Update(*this);
};

void FinalBossStage1::Render()
{
	state->Render(*this);
	this->w = this->currentFrameW;
	this->h = this->currentFrameH;

	if (updateState)
	{
		ChangeState(state, updateState, this);
		updateState = NULL;
	}
};

void FinalBossStage1::SetGun1(GunBossStage1* _gun1)
{
	gun1 = _gun1;
}

void FinalBossStage1::SetGun2(GunBossStage1* _gun2)
{
	gun2 = _gun2;
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
	if (HasSprites::hasBeenLoaded.value) {
		return;
	}
	HasSprites::hasBeenLoaded.value = true;

	GraphicsHelper::InsertSprite(BOSS_STAGE_1_SPRITE_ID::FINAL_BOSS_01, 54, 0, 23, 84, DIRECTION::LEFT, BOSS_STAGE_1_TEXTURE_ID::BOSS_STAGE_1);
	GraphicsHelper::InsertSprite(BOSS_STAGE_1_SPRITE_ID::FINAL_BOSS_02, 54, 27, 50, 84, DIRECTION::LEFT, BOSS_STAGE_1_TEXTURE_ID::BOSS_STAGE_1);
	GraphicsHelper::InsertSprite(BOSS_STAGE_1_SPRITE_ID::FINAL_BOSS_03, 54, 54, 77, 84, DIRECTION::LEFT, BOSS_STAGE_1_TEXTURE_ID::BOSS_STAGE_1);

	GraphicsHelper::InsertSprite(BOSS_STAGE_1_SPRITE_ID::FINAL_BOSS_DESTROY, 0, 0, 108, 38, DIRECTION::LEFT, BOSS_STAGE_1_TEXTURE_ID::BOSS_STAGE_1);
};

void FinalBossStage1::LoadAnimations()
{
	if (HasAnimations::hasBeenLoaded.value) {
		return;
	}
	HasAnimations::hasBeenLoaded.value = true;

	GraphicsHelper::InsertAnimation(BOSS_STAGE_1_ANIMATION_ID::FINAL_BOSS_NORMAL, 165,
		{
			{BOSS_STAGE_1_SPRITE_ID::FINAL_BOSS_01, 0},
			{BOSS_STAGE_1_SPRITE_ID::FINAL_BOSS_02, 0},
			{BOSS_STAGE_1_SPRITE_ID::FINAL_BOSS_03, 0},
		});

	GraphicsHelper::InsertAnimation(BOSS_STAGE_1_ANIMATION_ID::FINAL_BOSS_DESTROY, 165,
		{
			{BOSS_STAGE_1_SPRITE_ID::FINAL_BOSS_DESTROY, 0},
		});
};
