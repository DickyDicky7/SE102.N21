#include "Soldier.h"

Soldier::Soldier() : Entity(), HasTextures(), HasSprites(), HasAnimations()
{
	self = this;

	this->vx = 1;
	this->vy = 1;
	this->ax = 0.1;
	this->ay = 0.1;
	this->position.x = SCREEN_WIDTH / 4;
	this->position.y = SCREEN_HEIGHT / 2 - 50;

	this->updateState = NULL;
	this->handleInputState = NULL;
	this->state = new SoldierRunState();
}

Soldier::~Soldier()
{
}

void Soldier::Update()
{
	updateState = state->Update(*this);
}

void Soldier::Render()
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
	if (handleInputState)
	{
		state->Exit(*this);
		delete state;
		state = handleInputState;
		state->Enter(*this);
		handleInputState = NULL;
	}
}

void Soldier::HandleInput(Input& input)
{
	handleInputState = state->HandleInput(*this, input);
}

void Soldier::LoadSprites()
{
	if (HasSprites<Soldier>::hasBeenLoaded.value) return;
	HasSprites<Soldier>::hasBeenLoaded.value = true;

#pragma region Load Sprites

	// SPRITES
	GraphicsHelper::InsertSprite(SOLDIER_SPRITE_ID::RUN_01, 30, 1, 25, 65, BILL_TEXTURE_ID::BILL_01);


#pragma endregion Load Sprites

	OutputDebugString(L"Soldier Sprites Loaded Successfully\n");
}

void Soldier::LoadTextures()
{
	if (HasTextures<Soldier>::hasBeenLoaded.value) return;
	HasTextures<Soldier>::hasBeenLoaded.value = true;

	GraphicsHelper::InsertTexure(SOLDIER_TEXTURE_ID::SOLDIER_01, L"Resources\\Textures\\Soldier.png");

	OutputDebugString(L"Soldier Textures Loaded Successfully\n");
}

void Soldier::LoadAnimations()
{
	if (HasAnimations<Soldier>::hasBeenLoaded.value) return;
	HasAnimations<Soldier>::hasBeenLoaded.value = true;

#pragma region Load Animations

	GraphicsHelper::InsertAnimation(SOLDIER_ANIMATION_ID::RUN, 150,
		{
			{BILL_SPRITE_ID::NORMAL_01,0},
		});


#pragma endregion Load Animations

	OutputDebugString(L"Soldier Animations Loaded Successfully\n");
}