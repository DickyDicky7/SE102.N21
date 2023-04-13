#pragma once
#include "RifleManHideOnBush.h"

RifleManHideOnBush::RifleManHideOnBush() : Entity(), HasAnimations()
{
	Enemy::self = this;
	Entity::self = this;

	this->vx = 1.0f;
	this->vy = 1.0f;
	this->ax = 0.1f;
	this->ay = 0.1f;
	this->position.x = SCREEN_WIDTH / 3;
	this->position.y = SCREEN_HEIGHT / 2 - 200;

	this->movingDirection = DIRECTION::LEFT;

	this->updateState = NULL;
	this->state = new RifleManHideOnBushHidingState();
}

RifleManHideOnBush::~RifleManHideOnBush()
{

}

const Bill* RifleManHideOnBush::GetEnemyTarget()
{
	return Enemy::target;
}

void RifleManHideOnBush::Update()
{
	updateState = state->Update(*this);
}

void RifleManHideOnBush::Render()
{
	state->Render(*this);
	if (updateState)
	{
		state->Exit(*this);
		delete state;
		state = updateState;
		state->Enter(*this);
		updateState = NULL;
	}
}

void RifleManHideOnBush::HandleInput(Input& input)
{

}

void RifleManHideOnBush::LoadTextures()
{
	RifleMan::LoadTextures();
}

void RifleManHideOnBush::LoadSprites()
{
	GraphicsHelper::InsertSprite(RIFLE_MAN_SPRITE_ID::APPEAR_01, 0, 128, 150, 38, DIRECTION::LEFT, RIFLE_MAN_TEXTURE_ID::RIFLE_MAN);
	GraphicsHelper::InsertSprite(RIFLE_MAN_SPRITE_ID::APPEAR_02, 0, 153, 176, 38, DIRECTION::LEFT, RIFLE_MAN_TEXTURE_ID::RIFLE_MAN);
	GraphicsHelper::InsertSprite(RIFLE_MAN_SPRITE_ID::HIDE, 0, 118, 125, 38, DIRECTION::LEFT, RIFLE_MAN_TEXTURE_ID::RIFLE_MAN);
}

void RifleManHideOnBush::LoadAnimations()
{
	GraphicsHelper::InsertAnimation(RIFLE_MAN_ANIMATION_ID::APPEAR, 150,
		{
			{RIFLE_MAN_SPRITE_ID::HIDE, 0},
			{RIFLE_MAN_SPRITE_ID::APPEAR_01, 0},
			{RIFLE_MAN_SPRITE_ID::APPEAR_02, 0},
		});

	GraphicsHelper::InsertAnimation(RIFLE_MAN_ANIMATION_ID::HIDE, 150,
		{
			{RIFLE_MAN_SPRITE_ID::APPEAR_02, 0},
			{RIFLE_MAN_SPRITE_ID::APPEAR_01, 0},
			{RIFLE_MAN_SPRITE_ID::HIDE, 0},
		});

	GraphicsHelper::InsertAnimation(RIFLE_MAN_ANIMATION_ID::HIDDING, 150,
		{
			{RIFLE_MAN_SPRITE_ID::HIDE, 0},
		});

	GraphicsHelper::InsertAnimation(RIFLE_MAN_ANIMATION_ID::STANDING, 150,
		{
			{RIFLE_MAN_SPRITE_ID::APPEAR_02, 0},
		});
}