#include "ScubaSoldier.h"

// Pess H button to show or hidden scuba soldier

ScubaSoldier::ScubaSoldier() : Entity(), HasTextures(), HasSprites(), HasAnimations()
{
	Enemy::self = this;

	this->vx = 1.0f;
	this->vy = 1.0f;
	this->ax = 0.1f;
	this->ay = 0.1f;
	this->position.x = 300;
	this->position.y = 0;
	this->name = L"ScubaSoldier\n";

	this->updateState = NULL;
	this->handleInputState = NULL;
	// set direction default is right
	this->movingDirection = DIRECTION::RIGHT;
	// set state begin is run
	this->state = new ScubaSoldierHiddenState();
}

ScubaSoldier::~ScubaSoldier()
{
}

void ScubaSoldier::Update()
{
	updateState = state->Update(*this);
}

void ScubaSoldier::Render()
{
	state->Render(*this);
	this->w = this->currentFrameW;
	this->h = this->currentFrameH;

	if (updateState)
	{
		state->Exit(*this);
		delete state;
		state = updateState;
		state->Enter(*this);
		updateState = NULL;
	}
	if (handleInputState)
	{
		state->Exit(*this);
		delete state;
		state = handleInputState;
		state->Enter(*this);
		handleInputState = NULL;
	}
}

void ScubaSoldier::HandleInput(Input& input)
{
	handleInputState = state->HandleInput(*this, input);
}

void InsertSpriteScuba(SPRITE_ID spriteId, INT left, INT top, INT right, INT bottom)
{
	// i write this function to shorten the fuction: GraphicsHelper
	GraphicsHelper::InsertSprite(spriteId, top, left, right, bottom, DIRECTION::RIGHT, SCUBA_SOLDIER_TEXTURE_ID::SCUBA_SOLDIER_01);
}

void ScubaSoldier::LoadSprites()
{
	if (HasSprites<ScubaSoldier>::hasBeenLoaded.value) return;
	HasSprites<ScubaSoldier>::hasBeenLoaded.value = true;

#pragma region Load Sprites

	// SPRITES

	InsertSpriteScuba(SCUBA_SOLDIER_SPRITE_ID::HIDDEN_01, 2, 0, 17, 30);

	InsertSpriteScuba(SCUBA_SOLDIER_SPRITE_ID::SHOOTING_01, 20, 0, 35, 30);

#pragma endregion Load Sprites

	OutputDebugString(L"ScubaSoldier Sprites Loaded Successfully\n");
}

void ScubaSoldier::LoadTextures()
{
	if (HasTextures<ScubaSoldier>::hasBeenLoaded.value) return;
	HasTextures<ScubaSoldier>::hasBeenLoaded.value = true;

	GraphicsHelper::InsertTexure(SCUBA_SOLDIER_TEXTURE_ID::SCUBA_SOLDIER_01, L"Resources\\Textures\\ScubaSoldier.bmp");

	OutputDebugString(L"ScubaSoldier Textures Loaded Successfully\n");
}

void ScubaSoldier::LoadAnimations()
{
	if (HasAnimations<ScubaSoldier>::hasBeenLoaded.value) return;
	HasAnimations<ScubaSoldier>::hasBeenLoaded.value = true;

#pragma region Load Animations

	GraphicsHelper::InsertAnimation(SCUBA_SOLDIER_ANIMATION_ID::HIDDEN, 150,
		{
			{SCUBA_SOLDIER_SPRITE_ID::HIDDEN_01,0},
		});
	GraphicsHelper::InsertAnimation(SCUBA_SOLDIER_ANIMATION_ID::SHOOTING, 150,
		{
			{SCUBA_SOLDIER_SPRITE_ID::SHOOTING_01,0},
		});

#pragma endregion Load Animations

	OutputDebugString(L"ScubaSoldier Animations Loaded Successfully\n");
}
