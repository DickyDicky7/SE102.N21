#include "Falcon.h"
#include "Item.h"

Falcon::Falcon(ITEM_TYPE ammoType) : Entity(), HasTextures(), HasSprites(), HasAnimations()
{
	this->_vx = Constants::Physics::DEFAULT_INITIAL_VELOCITY_X;
	this->_vy = Constants::Physics::DEFAULT_INITIAL_VELOCITY_Y;
	this->_ax = Constants::Physics::DEFAULT_INITIAL_ACCELERATION_X;
	this->_ay = Constants::Physics::DEFAULT_INITIAL_ACCELERATION_Y;
	this->_position.x = Constants::Enemies::Falcon::DEFAULT_SPAWN_X;
	this->_position.y = Constants::Enemies::Falcon::DEFAULT_SPAWN_Y;
	this->SetDebugName(L"Falcon\n");
	this->_billDistance = 0.0f;

	this->_updateState = nullptr;
	this->_handleInputState = nullptr;
	// set direction default is right
	this->_movingDirection = DIRECTION::RIGHT;
	// set state begin is run
	this->_state = new FalconCloseState();
	this->_currentState = FALCON_ANIMATION_ID::CLOSE;

	this->_ammoType = ammoType;

	this->_hitCounts = Constants::Enemies::Falcon::HEALTH_POINTS;
	this->_enemyType = ENEMY_TYPE::MACHINE;
}

Falcon::~Falcon()
{
	Destroy(this->_state);
	Destroy(this->_updateState);
	Destroy(this->_handleInputState);
}

void Falcon::SetAmmoType(ITEM_TYPE ammoType)
{
	this->_ammoType = ammoType;
}

ITEM_TYPE Falcon::GetAmmoType()
{
	return this->_ammoType;
}

Item* Falcon::CreateDroppedItem() const
{
	Item* item = new Item(this->_ammoType);
	item->SetX(this->GetX());
	item->SetY(this->GetY());
	return item;
}

void Falcon::Update()
{
	this->CalculateBillDistance();

	if (this->_billDistance > Constants::Enemies::Falcon::DETECT_DISTANCE)
	{
		if (this->_currentState != FALCON_ANIMATION_ID::OPENING && this->_currentState != FALCON_ANIMATION_ID::CLOSE)
		{
			ChangeState(this->_state, new FalconOpeningState(FALCON_ANIMATION_ID::CLOSE), this);
			this->_currentState = FALCON_ANIMATION_ID::OPENING;
		}
	}
	else {
		if (this->_currentState != FALCON_ANIMATION_ID::OPENING && this->_currentState != FALCON_ANIMATION_ID::OPEN)
		{
			ChangeState(this->_state, new FalconOpeningState(FALCON_ANIMATION_ID::OPEN), this);
			this->_currentState = FALCON_ANIMATION_ID::OPENING;
		}
	}

	DeferState(this->_updateState, this->_state->Update(*this));

	ApplyDeferredState(this->_state, this->_updateState, this);
	ApplyDeferredState(this->_state, this->_handleInputState, this);
}

void Falcon::CalculateBillDistance()
{
	this->_billDistance = this->CalculateTargetDistance(this);
}

void Falcon::Render()
{
	this->_state->Render(*this);
	this->_w = this->GetCurrentFrameW();
	this->_h = this->GetCurrentFrameH();
}

void Falcon::HandleInput(Input& input)
{
	DeferState(this->_handleInputState, this->_state->HandleInput(*this, input));
}

void InsertSpriteFalcon(SPRITE_ID spriteId, int left, int top, int right, int bottom)
{
	// i write this function to shorten the function: GraphicsHelper
	GraphicsHelper::InsertSprite(spriteId, top, left, right, bottom, DIRECTION::RIGHT, FALCON_TEXTURE_ID::FALCON);
}

void Falcon::LoadSprites()
{
	if (HasSprites<Falcon>::_hasBeenLoaded) return;
	HasSprites<Falcon>::_hasBeenLoaded = true;

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
	if (HasTextures<Falcon>::_hasBeenLoaded) return;
	HasTextures<Falcon>::_hasBeenLoaded = true;

	GraphicsHelper::InsertTexture(FALCON_TEXTURE_ID::FALCON, L"Resources\\Textures\\Falcon.bmp");

	OutputDebugString(L"Falcon Textures Loaded Successfully\n");
}

void Falcon::LoadAnimations()
{
	if (HasAnimations<Falcon>::_hasBeenLoaded) return;
	HasAnimations<Falcon>::_hasBeenLoaded = true;

#pragma region Load Animations

	GraphicsHelper::InsertAnimation(FALCON_ANIMATION_ID::CLOSE, Constants::Enemies::Falcon::ANIMATION_DELAY_MILLISECONDS,
		{
			{FALCON_SPRITE_ID::CLOSE_1,0},
		});

	GraphicsHelper::InsertAnimation(FALCON_ANIMATION_ID::OPENING, Constants::Enemies::Falcon::ANIMATION_DELAY_MILLISECONDS,
		{
			{FALCON_SPRITE_ID::OPENING_1,0},
			{FALCON_SPRITE_ID::OPENING_2,0},
			{FALCON_SPRITE_ID::OPENING_3,0},
		});
	GraphicsHelper::InsertAnimation(FALCON_ANIMATION_ID::OPEN, Constants::Enemies::Falcon::ANIMATION_DELAY_MILLISECONDS,
		{
			{FALCON_SPRITE_ID::OPEN_1,0},
			{FALCON_SPRITE_ID::OPEN_2,0},
			{FALCON_SPRITE_ID::OPEN_3,0},
		});
#pragma endregion Load Animations

	OutputDebugString(L"Falcon Animations Loaded Successfully\n");
}

