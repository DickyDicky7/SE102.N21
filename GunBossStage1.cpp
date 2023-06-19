#include "GunBossStage1.h"

GunBossStage1::GunBossStage1()
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

	//
	this->type = 1;
	this->name = L"Gun Boss Stage 1\n";
	//
	this->hitCounts = 8;
	this->enemyType = ENEMY_TYPE::BOSS;
}

GunBossStage1::GunBossStage1(int _type)
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

	//
	this->type = _type;
	this->name = L"Gun Boss Stage 1\n";
	//
	this->hitCounts = 8;
	this->enemyType = ENEMY_TYPE::BOSS;
}

int GunBossStage1::GetType()
{
	return type;
}

GunBossStage1::~GunBossStage1()
{

};

void GunBossStage1::Update()
{
	if (!state)
	{
		state = new GunBossStage1NormalState();
	}

	updateState = state->Update(*this);
};

void GunBossStage1::Render()
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

void GunBossStage1::HandleInput(Input&)
{

};

void GunBossStage1::LoadTextures()
{
	BossStage1::LoadTextures();
};

void GunBossStage1::LoadSprites()
{
	if (HasSprites::hasBeenLoaded.value) {
		return;
	}
	HasSprites::hasBeenLoaded.value = true;

	GraphicsHelper::InsertSprite(BOSS_STAGE_1_SPRITE_ID::GUN_BOSS_01, 42, 27, 40, 48, DIRECTION::LEFT, BOSS_STAGE_1_TEXTURE_ID::BOSS_STAGE_1);
	GraphicsHelper::InsertSprite(BOSS_STAGE_1_SPRITE_ID::GUN_BOSS_01_HALF, 42, 27, 39, 48, DIRECTION::LEFT, BOSS_STAGE_1_TEXTURE_ID::BOSS_STAGE_1);

	GraphicsHelper::InsertSprite(BOSS_STAGE_1_SPRITE_ID::GUN_BOSS_02, 42, 48, 59, 48, DIRECTION::LEFT, BOSS_STAGE_1_TEXTURE_ID::BOSS_STAGE_1);
	GraphicsHelper::InsertSprite(BOSS_STAGE_1_SPRITE_ID::GUN_BOSS_02_HALF, 42, 48, 58, 48, DIRECTION::LEFT, BOSS_STAGE_1_TEXTURE_ID::BOSS_STAGE_1);

	GraphicsHelper::InsertSprite(BOSS_STAGE_1_SPRITE_ID::GUN_BOSS_DESTROY_01, 42, 0, 15, 50, DIRECTION::LEFT, BOSS_STAGE_1_TEXTURE_ID::BOSS_STAGE_1);
	GraphicsHelper::InsertSprite(BOSS_STAGE_1_SPRITE_ID::GUN_BOSS_DESTROY_02, 42, 19, 23, 47, DIRECTION::LEFT, BOSS_STAGE_1_TEXTURE_ID::BOSS_STAGE_1);
};

void GunBossStage1::LoadAnimations()
{
	if (HasAnimations::hasBeenLoaded.value) {
		return;
	}
	HasAnimations::hasBeenLoaded.value = true;

	GraphicsHelper::InsertAnimation(BOSS_STAGE_1_ANIMATION_ID::GUN_BOSS_01, 165,
		{
			{BOSS_STAGE_1_SPRITE_ID::GUN_BOSS_01, 0},
		});

	GraphicsHelper::InsertAnimation(BOSS_STAGE_1_ANIMATION_ID::GUN_BOSS_01_HALF, 165,
		{
			{BOSS_STAGE_1_SPRITE_ID::GUN_BOSS_01_HALF, 0},
		});

	GraphicsHelper::InsertAnimation(BOSS_STAGE_1_ANIMATION_ID::GUN_BOSS_02, 165,
		{
			{BOSS_STAGE_1_SPRITE_ID::GUN_BOSS_02, 0},
		});

	GraphicsHelper::InsertAnimation(BOSS_STAGE_1_ANIMATION_ID::GUN_BOSS_02_HALF, 165,
		{
			{BOSS_STAGE_1_SPRITE_ID::GUN_BOSS_02_HALF, 0},
		});

	GraphicsHelper::InsertAnimation(BOSS_STAGE_1_ANIMATION_ID::GUN_BOSS_DESTROY_01, 165,
		{
			{BOSS_STAGE_1_SPRITE_ID::GUN_BOSS_DESTROY_01, 0},
		});

	GraphicsHelper::InsertAnimation(BOSS_STAGE_1_ANIMATION_ID::GUN_BOSS_DESTROY_02, 165,
		{
			{BOSS_STAGE_1_SPRITE_ID::GUN_BOSS_DESTROY_02, 0},
		});
};
