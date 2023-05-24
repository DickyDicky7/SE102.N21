#include "Soldier.h"

Soldier::Soldier(AMMO_TYPE type) : Entity(), HasTextures(), HasSprites(), HasAnimations()
{
	this->vx = 1.0f;
	this->vy = 1.0f;
	this->ax = 0.1f;
	this->ay = 0.1f;
	this->position.x = 100;
	this->position.y = 0;
	this->name = L"Soldier\n";

	this->updateState = NULL;
	this->handleInputState = NULL;
	// set direction default is right
	this->movingDirection = DIRECTION::RIGHT;
	// set state begin is run
	this->state = new SoldierRunState();

	this->_ammoType = type;
}


void Soldier::setAmmoType(AMMO_TYPE type)
{
	this->_ammoType = type;
}

AMMO_TYPE Soldier::getAmmoType()
{
	return this->_ammoType;
}

/// <summary>
/// this fuction is check is the Bill hit the wall left and right ?
/// </summary>
/// <returns></returns>
BOOL Soldier::IsHitWall() {
	BOOL isHited = 0;
	// check is hit the wall left
	bool isHitLeftWall = (GetX() <= 0) && (GetMovingDirection() == DIRECTION::LEFT);
	bool isHitRightWall = (GetX() >= SCREEN_WIDTH - 520) && (GetMovingDirection() == DIRECTION::RIGHT);

	if (!isHitLeftWall && !isHitRightWall) isHited = 1;

	return isHited;
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

void Soldier::HandleInput(Input& input)
{
	handleInputState = state->HandleInput(*this, input);
}

void InsertSpriteSoldier(SPRITE_ID spriteId, INT left, INT top, INT right, INT bottom)
{
	// i write this function to shorten the fuction: GraphicsHelper
	GraphicsHelper::InsertSprite(spriteId, top, left, right, bottom, DIRECTION::LEFT, SOLDIER_TEXTURE_ID::SOLDIER_01);
}

void Soldier::LoadSprites()
{
	if (HasSprites<Soldier>::hasBeenLoaded.value) return;
	HasSprites<Soldier>::hasBeenLoaded.value = true;

#pragma region Load Sprites

	// SPRITES
	InsertSpriteSoldier(SOLDIER_SPRITE_ID::RUN_01, 90, 0, 106, 32);
	InsertSpriteSoldier(SOLDIER_SPRITE_ID::RUN_02, 72, 0, 88, 32);
	InsertSpriteSoldier(SOLDIER_SPRITE_ID::RUN_03, 54, 0, 70, 32);
	InsertSpriteSoldier(SOLDIER_SPRITE_ID::RUN_04, 36, 0, 52, 32);
	InsertSpriteSoldier(SOLDIER_SPRITE_ID::RUN_05, 18, 0, 34, 32);
	InsertSpriteSoldier(SOLDIER_SPRITE_ID::RUN_06, 0, 0, 16, 32);

	InsertSpriteSoldier(SOLDIER_SPRITE_ID::JUMP_01, 108, 0, 124, 32);

	InsertSpriteSoldier(SOLDIER_SPRITE_ID::SHOOT_01, 126, 0, 150, 32);
	InsertSpriteSoldier(SOLDIER_SPRITE_ID::SHOOT_02, 152, 0, 176, 32);

	InsertSpriteSoldier(SOLDIER_SPRITE_ID::LAY_DOWN_01, 178, 0, 210, 32);

	InsertSpriteSoldier(SOLDIER_SPRITE_ID::DIE_01, 212, 0, 227, 32);

#pragma endregion Load Sprites

	OutputDebugString(L"Soldier Sprites Loaded Successfully\n");
}

void Soldier::LoadTextures()
{
	if (HasTextures<Soldier>::hasBeenLoaded.value) return;
	HasTextures<Soldier>::hasBeenLoaded.value = true;

	GraphicsHelper::InsertTexure(SOLDIER_TEXTURE_ID::SOLDIER_01, L"Resources\\Textures\\Soldier.bmp");

	OutputDebugString(L"Soldier Textures Loaded Successfully\n");
}

void Soldier::LoadAnimations()
{
	if (HasAnimations<Soldier>::hasBeenLoaded.value) return;
	HasAnimations<Soldier>::hasBeenLoaded.value = true;

#pragma region Load Animations

	GraphicsHelper::InsertAnimation(SOLDIER_ANIMATION_ID::RUN, 70,
		{
			{SOLDIER_SPRITE_ID::RUN_01,0},
			{SOLDIER_SPRITE_ID::RUN_02,0},
			{SOLDIER_SPRITE_ID::RUN_03,0},
			{SOLDIER_SPRITE_ID::RUN_04,0},
			{SOLDIER_SPRITE_ID::RUN_05,0},
			{SOLDIER_SPRITE_ID::RUN_06,0},
		});
	GraphicsHelper::InsertAnimation(SOLDIER_ANIMATION_ID::JUMP, 60,
		{
			{SOLDIER_SPRITE_ID::JUMP_01,0},
		});
	GraphicsHelper::InsertAnimation(SOLDIER_ANIMATION_ID::SHOOT, 150,
		{
			{SOLDIER_SPRITE_ID::SHOOT_01,0},
			{SOLDIER_SPRITE_ID::SHOOT_02,0},
		});
	GraphicsHelper::InsertAnimation(SOLDIER_ANIMATION_ID::LAY_DOWN, 150,
		{
			{SOLDIER_SPRITE_ID::LAY_DOWN_01,0},
		});
	GraphicsHelper::InsertAnimation(SOLDIER_ANIMATION_ID::DIE, 150,
		{
			{SOLDIER_SPRITE_ID::DIE_01,0},
		});

#pragma endregion Load Animations

	OutputDebugString(L"Soldier Animations Loaded Successfully\n");
}