﻿#include "Item.h"

Item::Item(ITEM_TYPE type) : Entity(), HasTextures(), HasSprites(), HasAnimations()
{
	this->vx = 1.0f;
	this->vy = 1.0f;
	this->ax = 0.1f;
	this->ay = 0.1f;
	this->position.x = 0;
	this->position.y = 0;
	this->name = L"Item\n";

	this->movingDirection = DIRECTION::LEFT;
	this->type = type;

	time = 0.0f;
	dt = 0.03f;
	v0 = 7.0f;
	θ = 80.0f;
}

Item::~Item()
{
	time = NULL;
	dt = NULL;
	v0 = NULL;
	θ = NULL;
}

void Item::Update()
{
	// chuyen dong nem xien
	FLOAT x = GetX();
	FLOAT y = GetY();

	Motion::ProjectileMotionInputParameters pip{ x, y, v0, θ, time, dt };

	// dung lai khi va cham
	if (y >= 120) 
	{
		auto pop = Motion::CalculateProjectileMotion(pip);
		SetX(pop.x);
		SetY(pop.y);
		time = pop.t;
	}
}

void Item::Render()
{
	this->w = this->currentFrameW;
	this->h = this->currentFrameH;
	switch (type) {
		case ITEM_TYPE::B:
			SetAnimation(ITEM_ANIMATION_ID::B_AMMO, GetPosition(), GetMovingDirection(), GetAngle());
		case ITEM_TYPE::F:
			SetAnimation(ITEM_ANIMATION_ID::F_AMMO, GetPosition(), GetMovingDirection(), GetAngle());
		case ITEM_TYPE::L:
			SetAnimation(ITEM_ANIMATION_ID::L_AMMO, GetPosition(), GetMovingDirection(), GetAngle());
		case ITEM_TYPE::M:
			SetAnimation(ITEM_ANIMATION_ID::M_AMMO, GetPosition(), GetMovingDirection(), GetAngle());
		case ITEM_TYPE::R:
			SetAnimation(ITEM_ANIMATION_ID::R_AMMO, GetPosition(), GetMovingDirection(), GetAngle());
		case ITEM_TYPE::S:
			SetAnimation(ITEM_ANIMATION_ID::S_AMMO, GetPosition(), GetMovingDirection(), GetAngle());
		case ITEM_TYPE::I:
			SetAnimation(ITEM_ANIMATION_ID::INVUL, GetPosition(), GetMovingDirection(), GetAngle());
	}
}

void Item::HandleInput(Input& input)
{
}

void InsertSpriteItem(SPRITE_ID spriteId, INT left, INT top, INT right, INT bottom)
{
	// i write this function to shorten the fuction: GraphicsHelper
	GraphicsHelper::InsertSprite(spriteId, top, left, right, bottom, DIRECTION::RIGHT, AIRCRAFT_TEXTURE_ID::AIRCRAFT_01);
}

void Item::LoadSprites()
{
	if (HasSprites<Item>::hasBeenLoaded.value) return;
	HasSprites<Item>::hasBeenLoaded.value = true;

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
	if (HasAnimations<Item>::hasBeenLoaded.value) return;
	HasAnimations<Item>::hasBeenLoaded.value = true;

#pragma region Load Animations

	GraphicsHelper::InsertAnimation(ITEM_ANIMATION_ID::B_AMMO, 150,
		{
			{ITEM_SPRITE_ID::B_AMMO_01,0},
		});
	GraphicsHelper::InsertAnimation(ITEM_ANIMATION_ID::F_AMMO, 150,
		{
			{ITEM_SPRITE_ID::F_AMMO_01,0},
		});
	GraphicsHelper::InsertAnimation(ITEM_ANIMATION_ID::L_AMMO, 150,
		{
			{ITEM_SPRITE_ID::L_AMMO_01,0},
		});
	GraphicsHelper::InsertAnimation(ITEM_ANIMATION_ID::M_AMMO, 150,
		{
			{ITEM_SPRITE_ID::M_AMMO_01,0},
		});
	GraphicsHelper::InsertAnimation(ITEM_ANIMATION_ID::R_AMMO, 150,
		{
			{ITEM_SPRITE_ID::R_AMMO_01,0},
		});
	GraphicsHelper::InsertAnimation(ITEM_ANIMATION_ID::S_AMMO, 150,
		{
			{ITEM_SPRITE_ID::S_AMMO_01,0},
		});
	GraphicsHelper::InsertAnimation(ITEM_ANIMATION_ID::INVUL, 150,
		{
			{ITEM_SPRITE_ID::INVUL_01,0},
			{ITEM_SPRITE_ID::INVUL_02,0},
			{ITEM_SPRITE_ID::INVUL_03,0},
		});
#pragma endregion Load Animations

	OutputDebugString(L"Item Animations Loaded Successfully\n");
}
