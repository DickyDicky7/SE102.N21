#include "Falcon.h"
// O: open
// P: close
#define	DISTANCE_DETECT 100.0f
Falcon::Falcon() : Entity(), HasTextures(), HasSprites(), HasAnimations()
{
	Enemy::self = this;

	this->vx = 1.0f;
	this->vy = 1.0f;
	this->ax = 0.1f;
	this->ay = 0.1f;
	this->position.x = 300;
	this->position.y = 100;
	this->name = L"Falcon\n";
	this->billDistance = 0.0f;

	this->updateState = NULL;
	this->handleInputState = NULL;
	// set direction default is right
	this->movingDirection = DIRECTION::RIGHT;
	// set state begin is run
	this->state = new FalconCloseState();
	this->currentState = FALCON_ANIMATION_ID::CLOSE;
}

Falcon::~Falcon()
{
}

void Falcon::Update()
{
	CalculateBillDistance();

	if (billDistance > DISTANCE_DETECT) 
	{
		//this->state = new FalconOpeningState(FALCON_ANIMATION_ID::CLOSE);
		if (currentState != FALCON_ANIMATION_ID::OPENING && currentState != FALCON_ANIMATION_ID::CLOSE) 
		{
			this->state = new FalconOpeningState(FALCON_ANIMATION_ID::CLOSE);
			currentState = FALCON_ANIMATION_ID::OPENING;
		}
	}
	else {
		//this->state = new FalconOpeningState(FALCON_ANIMATION_ID::OPEN);
		if (currentState != FALCON_ANIMATION_ID::OPENING && currentState != FALCON_ANIMATION_ID::OPEN) 
		{
			this->state = new FalconOpeningState(FALCON_ANIMATION_ID::OPEN);
			currentState = FALCON_ANIMATION_ID::OPENING;
		}
	}

	updateState = state->Update(*this);
}

void Falcon::CalculateBillDistance()
{
	float dx = +(this->GetPosition().x - Enemy::target->GetPosition().x);
	float dy = -(this->GetPosition().y - Enemy::target->GetPosition().y);

	billDistance = sqrt(dx * dx + dy * dy);
}

void Falcon::Render()
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

void Falcon::HandleInput(Input& input)
{
	handleInputState = state->HandleInput(*this, input);
}

void InsertSpriteFalcon(SPRITE_ID spriteId, INT left, INT top, INT right, INT bottom)
{
	// i write this function to shorten the fuction: GraphicsHelper
	GraphicsHelper::InsertSprite(spriteId, top, left, right, bottom, DIRECTION::RIGHT, FALCON_TEXTURE_ID::FALCON);
}

void Falcon::LoadSprites()
{
	if (HasSprites<Falcon>::hasBeenLoaded.value) return;
	HasSprites<Falcon>::hasBeenLoaded.value = true;

#pragma region Load Sprites

	// SPRITES

	InsertSpriteFalcon(FALCON_SPRITE_ID::CLOSE_1, 0, 0, 31, 31);

	InsertSpriteFalcon(FALCON_SPRITE_ID::OPENING_1, 34, 0, 65, 31);
	InsertSpriteFalcon(FALCON_SPRITE_ID::OPENING_2, 68, 0, 99, 31);
	InsertSpriteFalcon(FALCON_SPRITE_ID::OPENING_3, 102, 0, 133, 31);

	InsertSpriteFalcon(FALCON_SPRITE_ID::OPEN_1, 136, 0, 167, 31);
	InsertSpriteFalcon(FALCON_SPRITE_ID::OPEN_2, 170, 0, 201, 31);
	InsertSpriteFalcon(FALCON_SPRITE_ID::OPEN_3, 204, 0, 235, 31);

#pragma endregion Load Sprites

	OutputDebugString(L"Falcon Sprites Loaded Successfully\n");
}

void Falcon::LoadTextures()
{
	if (HasTextures<Falcon>::hasBeenLoaded.value) return;
	HasTextures<Falcon>::hasBeenLoaded.value = true;

	GraphicsHelper::InsertTexure(FALCON_TEXTURE_ID::FALCON, L"Resources\\Textures\\Falcon.bmp");

	OutputDebugString(L"Falcon Textures Loaded Successfully\n");
}

void Falcon::LoadAnimations()
{
	if (HasAnimations<Falcon>::hasBeenLoaded.value) return;
	HasAnimations<Falcon>::hasBeenLoaded.value = true;

#pragma region Load Animations

	GraphicsHelper::InsertAnimation(FALCON_ANIMATION_ID::CLOSE, 150,
		{
			{FALCON_SPRITE_ID::CLOSE_1,0},
		});

	GraphicsHelper::InsertAnimation(FALCON_ANIMATION_ID::OPENING, 150,
		{
			{FALCON_SPRITE_ID::OPENING_1,0},
			{FALCON_SPRITE_ID::OPENING_2,0},
			{FALCON_SPRITE_ID::OPENING_3,0},
		});
	GraphicsHelper::InsertAnimation(FALCON_ANIMATION_ID::OPEN, 150,
		{
			{FALCON_SPRITE_ID::OPEN_1,0},
			{FALCON_SPRITE_ID::OPEN_2,0},
			{FALCON_SPRITE_ID::OPEN_3,0},
		});
#pragma endregion Load Animations

	OutputDebugString(L"Falcon Animations Loaded Successfully\n");
}

