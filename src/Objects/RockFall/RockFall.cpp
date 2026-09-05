#include "RockFall.h"
#include "TerrainBlock.h"

RockFall::RockFall() : Entity(), HasTextures(), HasSprites(), HasAnimations()
{
	this->_timeDelayToFall = Constants::Enemies::RockFall::DELAY_TO_FALL_FRAMES;

	this->_vx = Constants::Physics::DEFAULT_INITIAL_VELOCITY_X;
	this->_vy = Constants::Physics::DEFAULT_INITIAL_VELOCITY_Y;
	this->_ax = Constants::Physics::DEFAULT_INITIAL_ACCELERATION_X;
	this->_ay = Constants::Physics::DEFAULT_INITIAL_ACCELERATION_Y;
	this->_position.x = Constants::Enemies::RockFall::DEFAULT_SPAWN_X;
	this->_position.y = Constants::Enemies::RockFall::DEFAULT_SPAWN_Y;
	this->SetDebugName(L"RockFall\n");

	this->_updateState = nullptr;
	this->_handleInputState = nullptr;
	// set direction default is right
	this->_movingDirection = DIRECTION::RIGHT;
	// set state begin is run
	this->_state = new RockFallNormalState();

	this->_hitCounts = Constants::Enemies::RockFall::HEALTH_POINTS;
	this->_enemyType = ENEMY_TYPE::MACHINE;

	CollidableEntity::_self = this;
}

RockFall::~RockFall()
{
	Destroy(this->_state);
	Destroy(this->_updateState);
	Destroy(this->_handleInputState);

	this->_timeDelayToFall = 0.0f;
}

void RockFall::Update()
{
	DeferState(this->_updateState, this->_state->Update(*this));

	ApplyDeferredState(this->_state, this->_updateState, this);
	ApplyDeferredState(this->_state, this->_handleInputState, this);
}

void RockFall::Render()
{
	this->_state->Render(*this);
	this->_w = this->GetCurrentFrameW();
	this->_h = this->GetCurrentFrameH();
}

void RockFall::HandleInput(Input& input)
{
	DeferState(this->_handleInputState, this->_state->HandleInput(*this, input));
}

void InsertSpriteRockFall(SPRITE_ID spriteId, int left, int top, int right, int bottom)
{
	// i write this function to shorten the function: GraphicsHelper
	GraphicsHelper::InsertSprite(spriteId, top, left, right, bottom, DIRECTION::RIGHT, ROCK_FALL_TEXTURE_ID::ROCK_FALL);
}

void RockFall::LoadSprites()
{
	if (HasSprites<RockFall>::_hasBeenLoaded) return;
	HasSprites<RockFall>::_hasBeenLoaded = true;

#pragma region Load Sprites

	// SPRITES

	InsertSpriteRockFall(ROCK_FALL_SPRITE_ID::NORMAL_01, 5, 2, 28, 28);

	InsertSpriteRockFall(ROCK_FALL_SPRITE_ID::FALL_01, 35, 4, 61, 27);
	InsertSpriteRockFall(ROCK_FALL_SPRITE_ID::FALL_02, 69, 2, 92, 28);
	InsertSpriteRockFall(ROCK_FALL_SPRITE_ID::FALL_03, 99, 4, 125, 27);

#pragma endregion Load Sprites

	OutputDebugString(L"RockFall Sprites Loaded Successfully\n");
}

void RockFall::LoadTextures()
{
	if (HasTextures<RockFall>::_hasBeenLoaded) return;
	HasTextures<RockFall>::_hasBeenLoaded = true;

	GraphicsHelper::InsertTexture(ROCK_FALL_TEXTURE_ID::ROCK_FALL, L"Resources\\Textures\\RockFall.bmp");

	OutputDebugString(L"RockFall Textures Loaded Successfully\n");
}

void RockFall::LoadAnimations()
{
	if (HasAnimations<RockFall>::_hasBeenLoaded) return;
	HasAnimations<RockFall>::_hasBeenLoaded = true;

#pragma region Load Animations

	GraphicsHelper::InsertAnimation(ROCK_FALL_ANIMATION_ID::NORMAL, Constants::Enemies::RockFall::ANIMATION_DELAY_MILLISECONDS,
		{
			{ROCK_FALL_SPRITE_ID::NORMAL_01,0},
		});
	GraphicsHelper::InsertAnimation(ROCK_FALL_ANIMATION_ID::FALL, Constants::Enemies::RockFall::ANIMATION_DELAY_MILLISECONDS,
		{
			{ROCK_FALL_SPRITE_ID::FALL_01,0},
			{ROCK_FALL_SPRITE_ID::FALL_02,0},
			{ROCK_FALL_SPRITE_ID::FALL_03,0},
		});
#pragma endregion Load Animations

	OutputDebugString(L"RockFall Animations Loaded Successfully\n");
}

void RockFall::StaticResolveNoCollision()
{
}

void RockFall::StaticResolveOnCollision(AABBSweepResult aabbSweepResult)
{
}

void RockFall::DynamicResolveNoCollision()
{
}

void RockFall::DynamicResolveOnCollision(AABBSweepResult aabbSweepResult)
{
	if (!aabbSweepResult.surfaceEntity)
		return;

	TERRAIN_BLOCK_TYPE terrainType = aabbSweepResult.surfaceEntity->GetTerrainType();
	switch (terrainType)
	{

	case TERRAIN_BLOCK_TYPE::THROUGHABLE:
	{
		if (aabbSweepResult.normalY == +1.0f)
		{
			if (this->_alreadyCollidedWithEntities.find(aabbSweepResult.surfaceEntity) == this->_alreadyCollidedWithEntities.end())
			{
				Sound::GetInstance()->Play("stonefailing", false, 1);
				this->_position.y += aabbSweepResult.enTime * this->_vy;
				this->_vy = +Constants::Enemies::RockFall::BOUNCE_REBOUND_VELOCITY_Y;
				this->_ay = Constants::Enemies::RockFall::BOUNCE_REBOUND_ACCELERATION_Y;
				this->_bouncedBack = true;
				this->_alreadyCollidedWithEntities.insert(aabbSweepResult.surfaceEntity);
			}
		}
		return;
	}
	break;

	case TERRAIN_BLOCK_TYPE::NON_THROUGHABLE:
	{
		if (aabbSweepResult.normalY == +1.0f)
		{
			if (this->_alreadyCollidedWithEntities.find(aabbSweepResult.surfaceEntity) == this->_alreadyCollidedWithEntities.end())
			{
				this->_position.y += aabbSweepResult.enTime * this->_vy;
				this->_vy = +Constants::Enemies::RockFall::BOUNCE_REBOUND_VELOCITY_Y;
				this->_ay = Constants::Enemies::RockFall::BOUNCE_REBOUND_ACCELERATION_Y;
				this->_bouncedBack = true;
				this->_alreadyCollidedWithEntities.insert(aabbSweepResult.surfaceEntity);
			}
		}
		return;
	}
	break;

	}
}
