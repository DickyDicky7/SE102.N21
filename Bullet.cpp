#include "Bill.h"
#include "Fire.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Bridge.h"
#include "Soldier.h"
#include "RockFly.h"
#include "TerrainBlock.h"
#include "GunBossStage1.h"
#include "BossStage3Hand.h"
#include "FinalBossStage1.h"
#include "BossStage3Joint.h"

Bullet::Bullet(                  ) : Entity(), HasTextures(), HasSprites(), HasAnimations(), CollidableEntity()
{
	this->isFake  = 0;
	this->isEnemy = 0;
	this->state = NULL;
	this->updateState = NULL;
	this->handleInputState = NULL;
	CollidableEntity::self = (Entity*)this;
}

Bullet::Bullet(BulletState* state) : Bullet()
{
	this->state = state;
}

Bullet::~Bullet()
{
	Destroy(state);
	Destroy(updateState);
	Destroy(handleInputState);
}

void Bullet::Update()
{
	updateState = state->Update(*this);
}

void Bullet::Render()
{
	state->Render(*this);
	this->w = this->currentFrameW;
	this->h = this->currentFrameH;

	if (updateState)
	{
		ChangeState(state, updateState, this);
		updateState = NULL;
	}
	if (handleInputState)
	{
		ChangeState(state, handleInputState, this);
		handleInputState = NULL;
	}
}

void Bullet::HandleInput(Input& input)
{
	handleInputState = state->HandleInput(*this, input);
}

void Bullet::SetState(BulletState* newState)
{
	ChangeState(state, newState, this);
	newState = NULL;
}

BulletState* Bullet::GetState() const
{
	return state;
}

void Bullet::LoadSprites()
{
	if (HasSprites<Bullet>::hasBeenLoaded.value) return;
	HasSprites<Bullet>::hasBeenLoaded.value = true;

	GraphicsHelper::InsertSprite(BULLET_SPRITE_ID::R_01, 42, 52, 55, 45, DIRECTION::LEFT, BULLET_TEXTURE_ID::BULLET_01);
	GraphicsHelper::InsertSprite(BULLET_SPRITE_ID::M_01, 28, 69, 74, 33, DIRECTION::LEFT, BULLET_TEXTURE_ID::BULLET_01);
	GraphicsHelper::InsertSprite(BULLET_SPRITE_ID::S_01, 28, 69, 74, 33, DIRECTION::LEFT, BULLET_TEXTURE_ID::BULLET_01);
	GraphicsHelper::InsertSprite(BULLET_SPRITE_ID::S_02, 27, 61, 67, 33, DIRECTION::LEFT, BULLET_TEXTURE_ID::BULLET_01);
	GraphicsHelper::InsertSprite(BULLET_SPRITE_ID::S_03, 25, 50, 58, 33, DIRECTION::LEFT, BULLET_TEXTURE_ID::BULLET_01);
	GraphicsHelper::InsertSprite(BULLET_SPRITE_ID::L_01, 39, 23, 38, 45, DIRECTION::LEFT, BULLET_TEXTURE_ID::BULLET_01);
	GraphicsHelper::InsertSprite(BULLET_SPRITE_ID::F_01, 37, 41, 49, 45, DIRECTION::LEFT, BULLET_TEXTURE_ID::BULLET_01);

	GraphicsHelper::InsertSprite(BULLET_SPRITE_ID::ENEMY_01, 42, 58, 60, 44, DIRECTION::LEFT, BULLET_TEXTURE_ID::BULLET_01);

	GraphicsHelper::InsertSprite(BULLET_SPRITE_ID::BOSS_1_01, 25, 50, 58, 33, DIRECTION::LEFT, BULLET_TEXTURE_ID::BULLET_01);
	GraphicsHelper::InsertSprite(BULLET_SPRITE_ID::BOSS_2_01, 00, 00, 15, 15, DIRECTION::LEFT, BULLET_TEXTURE_ID::BULLET_01);
	GraphicsHelper::InsertSprite(BULLET_SPRITE_ID::BOSS_2_02, 00, 19, 34, 15, DIRECTION::LEFT, BULLET_TEXTURE_ID::BULLET_01);
	GraphicsHelper::InsertSprite(BULLET_SPRITE_ID::BOSS_2_03, 00, 37, 52, 15, DIRECTION::LEFT, BULLET_TEXTURE_ID::BULLET_01);
	GraphicsHelper::InsertSprite(BULLET_SPRITE_ID::BOSS_2_04, 00, 55, 70, 15, DIRECTION::LEFT, BULLET_TEXTURE_ID::BULLET_01);

	GraphicsHelper::InsertSprite(BULLET_SPRITE_ID::EXPLODE_01, 25, 41, 46, 32, DIRECTION::LEFT, BULLET_TEXTURE_ID::BULLET_01);

	OutputDebugString(L"Bullet Sprites Loaded Successfully\n");
}

void Bullet::LoadTextures()
{
	if (HasTextures<Bullet>::hasBeenLoaded.value) return;
	HasTextures<Bullet>::hasBeenLoaded.value = true;

	GraphicsHelper::InsertTexure(BULLET_TEXTURE_ID::BULLET_01, L"Resources\\Textures\\Bullet.bmp");

	OutputDebugString(L"Bullet Textures Loaded Successfully\n");
}

void Bullet::LoadAnimations()
{
	if (HasAnimations<Bullet>::hasBeenLoaded.value) return;
	HasAnimations<Bullet>::hasBeenLoaded.value = true;

	GraphicsHelper::InsertAnimation(BULLET_ANIMATION_ID::R, 150,
		{
			{BULLET_SPRITE_ID::R_01, 0},
		});

	GraphicsHelper::InsertAnimation(BULLET_ANIMATION_ID::M, 150,
		{
			{BULLET_SPRITE_ID::M_01, 0},
		});

	GraphicsHelper::InsertAnimation(BULLET_ANIMATION_ID::S, 400,
		{
			{BULLET_SPRITE_ID::S_01, 0},
			{BULLET_SPRITE_ID::S_02, 0},
			{BULLET_SPRITE_ID::S_03, 0},
		});

	GraphicsHelper::InsertAnimation(BULLET_ANIMATION_ID::L, 150,
		{
			{BULLET_SPRITE_ID::L_01, 0},
		});

	GraphicsHelper::InsertAnimation(BULLET_ANIMATION_ID::F, 150,
		{
			{BULLET_SPRITE_ID::F_01, 0},
		});

	GraphicsHelper::InsertAnimation(BULLET_ANIMATION_ID::ENEMY, 150,
		{
			{BULLET_SPRITE_ID::ENEMY_01, 0},
		});

	GraphicsHelper::InsertAnimation(BULLET_ANIMATION_ID::BOSS_1, 150,
		{
			{BULLET_SPRITE_ID::BOSS_1_01, 0},
		});

	GraphicsHelper::InsertAnimation(BULLET_ANIMATION_ID::BOSS_2, 150,
		{
			{BULLET_SPRITE_ID::BOSS_2_01, 0},
			{BULLET_SPRITE_ID::BOSS_2_02, 0},
			{BULLET_SPRITE_ID::BOSS_2_03, 0},
			{BULLET_SPRITE_ID::BOSS_2_04, 0},
		});

	GraphicsHelper::InsertAnimation(BULLET_ANIMATION_ID::EXPLODE, 150,
		{
			{BULLET_SPRITE_ID::EXPLODE_01, 0},
		});

	OutputDebugString(L"Bullet Animations Loaded Successfully\n");
}

void Bullet::StaticResolveNoCollision(                               )
{
}

void Bullet::StaticResolveOnCollision(AABBSweepResult aabbSweepResult)
{
}

void Bullet::DynamicResolveNoCollision(                               )
{
}

void Bullet::DynamicResolveOnCollision(AABBSweepResult aabbSweepResult)
{
	auto rockfly = dynamic_cast<RockFly*>(aabbSweepResult.surfaceEntity);
	if  (rockfly)
	{
		return;
	}

	auto bridge = dynamic_cast<Bridge*>(aabbSweepResult.surfaceEntity);
	if  (bridge)
	{
		return;
	}

	auto enemy = dynamic_cast<Enemy<Bill>*>(aabbSweepResult.surfaceEntity);
	if  (enemy)
	{
		if (isEnemy)
		{
			return;
		}

		auto fire = dynamic_cast<Fire*>(aabbSweepResult.surfaceEntity);
		if  (fire)
		{
			return;
		}

		auto gunBossStage1 = dynamic_cast<GunBossStage1*>(aabbSweepResult.surfaceEntity);
		if  (gunBossStage1 
		&&   gunBossStage1->isDead)
		{
			return;
		}

		auto finalBossStage1 = dynamic_cast<FinalBossStage1*>(aabbSweepResult.surfaceEntity);
		if  (finalBossStage1 
		&&   finalBossStage1->isDead)
		{
			return;
		}

		isDead = 1;
		Sound::getInstance()->play("beShooted", false, 1);
		auto bossStage3Joint = dynamic_cast<BossStage3Joint*>(aabbSweepResult.surfaceEntity);
		if  (bossStage3Joint)
		{
			if (  bossStage3Joint->parent 
			&&  --bossStage3Joint->parent->hitCounts == 0)
				  bossStage3Joint->parent->isDead     = 1;
		}
		else
		if  (--enemy->hitCounts == 0)
		{
			if (auto soldier = dynamic_cast<Soldier*>(enemy)) soldier->GoDead();
			else aabbSweepResult.surfaceEntity->isDead = 1;
		}
		return;
	}

	auto bill = dynamic_cast<Bill*>(aabbSweepResult.surfaceEntity);
	if  (bill
	&&   isEnemy)
	{
		 bill->GoDead(); isDead = 1; 
		 return;
	}

	if (dynamic_cast<BulletScubaSoldierState*>(state))
	{
		auto    terrainBlock = dynamic_cast<TerrainBlock*>(aabbSweepResult.surfaceEntity);
		if     (terrainBlock)
		{
		if     (aabbSweepResult.normalY == +1.0f)
		switch (terrainBlock->type)
		{
		case TERRAIN_BLOCK_TYPE::    THROUGHABLE:
		case TERRAIN_BLOCK_TYPE::NON_THROUGHABLE:
			 isDead = 1;
		break;
		}
		}
		return;
	}

	if (dynamic_cast<BulletBossStage1State*>(state))
	{
		auto    terrainBlock = dynamic_cast<TerrainBlock*>(aabbSweepResult.surfaceEntity);
		if     (terrainBlock)
		{
		if     (aabbSweepResult.normalY == +1.0f)
		switch (terrainBlock->type)
		{
		case TERRAIN_BLOCK_TYPE::NON_THROUGHABLE:
			 isDead = 1;
		break;
		}
		}
		return;
	}
}
