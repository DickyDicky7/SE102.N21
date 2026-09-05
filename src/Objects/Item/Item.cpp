#include "Item.h"
#include "TerrainBlock.h"

Item::Item(ITEM_TYPE type) : Entity(), HasTextures(), HasSprites(), HasAnimations()
{
	this->_vx = Constants::Physics::DEFAULT_INITIAL_VELOCITY_X;
	this->_vy = Constants::Physics::DEFAULT_INITIAL_VELOCITY_Y;
	this->_ax = Constants::Physics::DEFAULT_INITIAL_ACCELERATION_X;
	this->_ay = Constants::Physics::DEFAULT_INITIAL_ACCELERATION_Y;
	this->_position.x = 0;
	this->_position.y = 0;
	this->SetDebugName(L"Item\n");

	this->_movingDirection = DIRECTION::LEFT;
	this->_type = type;

	this->_time = 0.0f;
	this->_dt = Constants::Objects::Item::POP_DELTA_TIME;
	this->_v0 = Constants::Objects::Item::POP_INITIAL_VELOCITY;
	this->_theta = Constants::Objects::Item::POP_LAUNCH_ANGLE_DEGREES;

	this->_stopUpdate = false;
	CollidableEntity::_self = this;
}

Item::~Item()
{
}

void Item::Update()
{
	if (!this->_stopUpdate)
	{
		// Projectile motion
		float x = this->GetX();
		float y = this->GetY();

		Motion::ProjectileMotionInputParameters pip{ x, y, this->_v0, this->_theta, this->_time, this->_dt };

		auto pop = Motion::CalculateProjectileMotion(pip);
		this->SetX(pop.positionX);
		this->SetY(pop.positionY);
		this->SetVX(pop.velocityX);
		this->SetVY(pop.velocityY);
		this->_time = pop.elapsedTime;
	}
}

void Item::Render()
{
	this->_w = this->GetCurrentFrameW();
	this->_h = this->GetCurrentFrameH();

	ITEM_ANIMATION_ID animId = ITEM_ANIMATION_ID::B_AMMO;
	switch (this->_type)
	{
	case ITEM_TYPE::B: animId = ITEM_ANIMATION_ID::B_AMMO; break;
	case ITEM_TYPE::F: animId = ITEM_ANIMATION_ID::F_AMMO; break;
	case ITEM_TYPE::L: animId = ITEM_ANIMATION_ID::L_AMMO; break;
	case ITEM_TYPE::M: animId = ITEM_ANIMATION_ID::M_AMMO; break;
	case ITEM_TYPE::R: animId = ITEM_ANIMATION_ID::R_AMMO; break;
	case ITEM_TYPE::S: animId = ITEM_ANIMATION_ID::S_AMMO; break;
	case ITEM_TYPE::I: animId = ITEM_ANIMATION_ID::INVUL; break;
	default: break;
	}
	this->SetAnimation(animId, this->GetPosition(), this->GetMovingDirection(), this->GetAngle());
}

void Item::HandleInput(Input& input)
{
}

void InsertSpriteItem(SPRITE_ID spriteId, int left, int top, int right, int bottom)
{
	// i write this function to shorten the function: GraphicsHelper
	GraphicsHelper::InsertSprite(spriteId, top, left, right, bottom, DIRECTION::LEFT, AIRCRAFT_TEXTURE_ID::AIRCRAFT_01);
}

void Item::LoadSprites()
{
	if (HasSprites<Item>::_hasBeenLoaded) return;
	HasSprites<Item>::_hasBeenLoaded = true;

#pragma region Load Sprites

	// SPRITES

	InsertSpriteItem(ITEM_SPRITE_ID::B_AMMO_01, 28, 0, 53, 15);
	InsertSpriteItem(ITEM_SPRITE_ID::F_AMMO_01, 54, 0, 79, 15);
	InsertSpriteItem(ITEM_SPRITE_ID::L_AMMO_01, 80, 0, 105, 15);
	InsertSpriteItem(ITEM_SPRITE_ID::M_AMMO_01, 106, 0, 131, 15);
	InsertSpriteItem(ITEM_SPRITE_ID::R_AMMO_01, 132, 0, 157, 15);
	InsertSpriteItem(ITEM_SPRITE_ID::S_AMMO_01, 158, 0, 183, 15);

	InsertSpriteItem(ITEM_SPRITE_ID::INVUL_01, 186, 0, 212, 15);
	InsertSpriteItem(ITEM_SPRITE_ID::INVUL_02, 217, 0, 242, 15);
	InsertSpriteItem(ITEM_SPRITE_ID::INVUL_03, 247, 0, 272, 15);

#pragma endregion Load Sprites

	OutputDebugString(L"Item Sprites Loaded Successfully\n");
}

void Item::LoadTextures()
{
	// load in aircraft
}

void Item::LoadAnimations()
{
	if (HasAnimations<Item>::_hasBeenLoaded) return;
	HasAnimations<Item>::_hasBeenLoaded = true;

#pragma region Load Animations

	GraphicsHelper::InsertAnimation(ITEM_ANIMATION_ID::B_AMMO, Constants::Objects::Item::ANIMATION_DELAY_MILLISECONDS,
		{
			{ITEM_SPRITE_ID::B_AMMO_01,0},
		});
	GraphicsHelper::InsertAnimation(ITEM_ANIMATION_ID::F_AMMO, Constants::Objects::Item::ANIMATION_DELAY_MILLISECONDS,
		{
			{ITEM_SPRITE_ID::F_AMMO_01,0},
		});
	GraphicsHelper::InsertAnimation(ITEM_ANIMATION_ID::L_AMMO, Constants::Objects::Item::ANIMATION_DELAY_MILLISECONDS,
		{
			{ITEM_SPRITE_ID::L_AMMO_01,0},
		});
	GraphicsHelper::InsertAnimation(ITEM_ANIMATION_ID::M_AMMO, Constants::Objects::Item::ANIMATION_DELAY_MILLISECONDS,
		{
			{ITEM_SPRITE_ID::M_AMMO_01,0},
		});
	GraphicsHelper::InsertAnimation(ITEM_ANIMATION_ID::R_AMMO, Constants::Objects::Item::ANIMATION_DELAY_MILLISECONDS,
		{
			{ITEM_SPRITE_ID::R_AMMO_01,0},
		});
	GraphicsHelper::InsertAnimation(ITEM_ANIMATION_ID::S_AMMO, Constants::Objects::Item::ANIMATION_DELAY_MILLISECONDS,
		{
			{ITEM_SPRITE_ID::S_AMMO_01,0},
		});
	GraphicsHelper::InsertAnimation(ITEM_ANIMATION_ID::INVUL, Constants::Objects::Item::ANIMATION_DELAY_MILLISECONDS,
		{
			{ITEM_SPRITE_ID::INVUL_01,0},
			{ITEM_SPRITE_ID::INVUL_02,0},
			{ITEM_SPRITE_ID::INVUL_03,0},
		});
#pragma endregion Load Animations

	OutputDebugString(L"Item Animations Loaded Successfully\n");
}

void Item::StaticResolveNoCollision()
{
}

void Item::StaticResolveOnCollision(AABBSweepResult aabbSweepResult)
{
}

void Item::DynamicResolveNoCollision()
{
}

void Item::DynamicResolveOnCollision(AABBSweepResult aabbSweepResult)
{
	if (aabbSweepResult.surfaceEntity && aabbSweepResult.surfaceEntity->GetTerrainType() != TERRAIN_BLOCK_TYPE::NONE)
	{
		if (aabbSweepResult.surfaceEntity->GetY() <= this->_position.y)
		{
			this->_position.y = aabbSweepResult.surfaceEntity->GetT();
			this->_stopUpdate = true;
		}
	}
}

bool Item::OnBillCollision(Bill& bill, const AABBSweepResult& result)
{
	this->SetDead(true);
	bill.CollectItem(this->_type);
	return true;
}
