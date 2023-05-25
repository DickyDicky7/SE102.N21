#include "AirCraft.h"

AirCraft::AirCraft(ITEM_TYPE type, AIRCRAFT_DIRECTION direction) : Entity(), HasTextures(), HasSprites(), HasAnimations()
{

	this->vx = 1.0f;
	this->vy = 0.5f;
	this->ax = 0.1f;
	this->ay = 0.1f;
	this->position.x = 100;
	this->position.y = 200;
	this->name = L"AirCraft\n";

	this->updateState = NULL;
	this->handleInputState = NULL;
	// set direction default is right
	this->movingDirection = DIRECTION::RIGHT;
	// set state begin is run
	this->state = new AirCraftNormalState();

	this->_ammoType = type;
	this->_aircarftDirection = direction;
}

AirCraft::~AirCraft()
{
}

void AirCraft::setAmmoType(ITEM_TYPE type) 
{
	this->_ammoType = type;
}

ITEM_TYPE AirCraft::getAmmoType() 
{
	return this->_ammoType;
}

void AirCraft::Update()
{
	updateState = state->Update(*this);
}

void AirCraft::Render()
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

void AirCraft::HandleInput(Input& input)
{
	handleInputState = state->HandleInput(*this, input);
}

void InsertSpriteAirCraft(SPRITE_ID spriteId, INT left, INT top, INT right, INT bottom)
{
	// i write this function to shorten the fuction: GraphicsHelper
	GraphicsHelper::InsertSprite(spriteId, top, left, right, bottom, DIRECTION::RIGHT, AIRCRAFT_TEXTURE_ID::AIRCRAFT_01);
}

void AirCraft::LoadSprites()
{
	if (HasSprites<AirCraft>::hasBeenLoaded.value) return;
	HasSprites<AirCraft>::hasBeenLoaded.value = true;

#pragma region Load Sprites

	// SPRITES

	InsertSpriteAirCraft(AIRCRAFT_SPRITE_ID::NORMAL_01, 2, 0, 27, 15);

	InsertSpriteAirCraft(AIRCRAFT_SPRITE_ID::B_AMMO_01, 28, 0, 53, 15);
	InsertSpriteAirCraft(AIRCRAFT_SPRITE_ID::F_AMMO_01, 54, 0, 79, 15);
	InsertSpriteAirCraft(AIRCRAFT_SPRITE_ID::L_AMMO_01, 80, 0, 105, 15);
	InsertSpriteAirCraft(AIRCRAFT_SPRITE_ID::M_AMMO_01, 106, 0, 131, 15);
	InsertSpriteAirCraft(AIRCRAFT_SPRITE_ID::R_AMMO_01, 132, 0, 157, 15);
	InsertSpriteAirCraft(AIRCRAFT_SPRITE_ID::S_AMMO_01, 158, 0, 183, 15);

	InsertSpriteAirCraft(AIRCRAFT_SPRITE_ID::INVUL_01, 186, 0, 212, 15);
	InsertSpriteAirCraft(AIRCRAFT_SPRITE_ID::INVUL_02, 217, 0, 242, 15);
	InsertSpriteAirCraft(AIRCRAFT_SPRITE_ID::INVUL_03, 247, 0, 272, 15);

#pragma endregion Load Sprites

	OutputDebugString(L"AirCraft Sprites Loaded Successfully\n");
}

void AirCraft::LoadTextures()
{
	if (HasTextures<AirCraft>::hasBeenLoaded.value) return;
	HasTextures<AirCraft>::hasBeenLoaded.value = true;

	GraphicsHelper::InsertTexure(AIRCRAFT_TEXTURE_ID::AIRCRAFT_01, L"Resources\\Textures\\Aircraft.bmp");

	OutputDebugString(L"AirCraft Textures Loaded Successfully\n");
}

void AirCraft::LoadAnimations()
{
	if (HasAnimations<AirCraft>::hasBeenLoaded.value) return;
	HasAnimations<AirCraft>::hasBeenLoaded.value = true;

#pragma region Load Animations

	GraphicsHelper::InsertAnimation(AIRCRAFT_ANIMATION_ID::NORMAL, 150,
		{
			{AIRCRAFT_SPRITE_ID::NORMAL_01,0},
		});

	GraphicsHelper::InsertAnimation(AIRCRAFT_ANIMATION_ID::B_AMMO, 150,
		{
			{AIRCRAFT_SPRITE_ID::B_AMMO_01,0},
		});
	GraphicsHelper::InsertAnimation(AIRCRAFT_ANIMATION_ID::F_AMMO, 150,
		{
			{AIRCRAFT_SPRITE_ID::F_AMMO_01,0},
		});
	GraphicsHelper::InsertAnimation(AIRCRAFT_ANIMATION_ID::L_AMMO, 150,
		{
			{AIRCRAFT_SPRITE_ID::L_AMMO_01,0},
		});
	GraphicsHelper::InsertAnimation(AIRCRAFT_ANIMATION_ID::M_AMMO, 150,
		{
			{AIRCRAFT_SPRITE_ID::M_AMMO_01,0},
		});
	GraphicsHelper::InsertAnimation(AIRCRAFT_ANIMATION_ID::R_AMMO, 150,
		{
			{AIRCRAFT_SPRITE_ID::R_AMMO_01,0},
		});
	GraphicsHelper::InsertAnimation(AIRCRAFT_ANIMATION_ID::S_AMMO, 150,
		{
			{AIRCRAFT_SPRITE_ID::S_AMMO_01,0},
		});

	GraphicsHelper::InsertAnimation(AIRCRAFT_ANIMATION_ID::INVUL, 150,
		{
			{AIRCRAFT_SPRITE_ID::INVUL_01,0},
			{AIRCRAFT_SPRITE_ID::INVUL_02,0},
			{AIRCRAFT_SPRITE_ID::INVUL_03,0},
		});

#pragma endregion Load Animations

	OutputDebugString(L"AirCraft Animations Loaded Successfully\n");
}