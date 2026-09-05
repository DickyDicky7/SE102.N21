#include "Explosion.h"
// O: open
// P: close

Explosion::Explosion() : Entity(), HasTextures(), HasSprites(), HasAnimations()
{
	this->_vx = Constants::Physics::DEFAULT_INITIAL_VELOCITY_X;
	this->_vy = Constants::Physics::DEFAULT_INITIAL_VELOCITY_Y;
	this->_ax = Constants::Physics::DEFAULT_INITIAL_ACCELERATION_X;
	this->_ay = Constants::Physics::DEFAULT_INITIAL_ACCELERATION_Y;
	this->_position.x = Constants::Objects::Explosion::DEFAULT_SPAWN_X;
	this->_position.y = Constants::Objects::Explosion::DEFAULT_SPAWN_Y;
	this->SetDebugName(L"Explosion\n");

	this->_updateState = nullptr;
	this->_handleInputState = nullptr;
	this->_movingDirection = DIRECTION::RIGHT;
	this->_state = new ExplosionType1State();
}

Explosion::Explosion(ExplosionState* state) : Entity(), HasTextures(), HasSprites(), HasAnimations()
{
	this->_vx = Constants::Physics::DEFAULT_INITIAL_VELOCITY_X;
	this->_vy = Constants::Physics::DEFAULT_INITIAL_VELOCITY_Y;
	this->_ax = Constants::Physics::DEFAULT_INITIAL_ACCELERATION_X;
	this->_ay = Constants::Physics::DEFAULT_INITIAL_ACCELERATION_Y;
	this->_position.x = Constants::Objects::Explosion::DEFAULT_SPAWN_X;
	this->_position.y = Constants::Objects::Explosion::DEFAULT_SPAWN_Y;
	this->SetDebugName(L"Explosion\n");

	this->_updateState = nullptr;
	this->_handleInputState = nullptr;
	this->_movingDirection = DIRECTION::RIGHT;
	this->_state = state;
}

Explosion::~Explosion()
{
	Destroy(this->_state);
	Destroy(this->_updateState);
	Destroy(this->_handleInputState);
}

void Explosion::Update()
{
	DeferState(this->_updateState, this->_state->Update(*this));

	ApplyDeferredState(this->_state, this->_updateState, this);
	ApplyDeferredState(this->_state, this->_handleInputState, this);
}

void Explosion::Render()
{
	this->_state->Render(*this);
	this->_w = this->GetCurrentFrameW();
	this->_h = this->GetCurrentFrameH();
}

void Explosion::HandleInput(Input& input)
{
	DeferState(this->_handleInputState, this->_state->HandleInput(*this, input));
}

void InsertSpriteExplosion(SPRITE_ID spriteId, int left, int top, int right, int bottom)
{
	// i write this function to shorten the function: GraphicsHelper
	GraphicsHelper::InsertSprite(spriteId, top, left, right, bottom, DIRECTION::RIGHT, EXPLOSION_TEXTURE_ID::EXPLOSION);
}

void Explosion::LoadSprites()
{
	if (HasSprites<Explosion>::_hasBeenLoaded) return;
	HasSprites<Explosion>::_hasBeenLoaded = true;

#pragma region Load Sprites

	// SPRITES

	InsertSpriteExplosion(EXPLOSION_SPRITE_ID::TYPE_1_01, 0, 0, 25, 32);
	InsertSpriteExplosion(EXPLOSION_SPRITE_ID::TYPE_1_02, 164, 0, 189, 32);
	InsertSpriteExplosion(EXPLOSION_SPRITE_ID::TYPE_1_03, 26, 0, 55, 32);
	InsertSpriteExplosion(EXPLOSION_SPRITE_ID::TYPE_1_04, 190, 0, 219, 32);
	InsertSpriteExplosion(EXPLOSION_SPRITE_ID::TYPE_1_05, 56, 0, 89, 32);
	InsertSpriteExplosion(EXPLOSION_SPRITE_ID::TYPE_1_06, 220, 0, 253, 32);

	InsertSpriteExplosion(EXPLOSION_SPRITE_ID::TYPE_2_01, 90, 0, 106, 32);
	InsertSpriteExplosion(EXPLOSION_SPRITE_ID::TYPE_2_02, 254, 0, 270, 32);
	InsertSpriteExplosion(EXPLOSION_SPRITE_ID::TYPE_2_03, 107, 0, 131, 32);
	InsertSpriteExplosion(EXPLOSION_SPRITE_ID::TYPE_2_04, 271, 0, 295, 32);
	InsertSpriteExplosion(EXPLOSION_SPRITE_ID::TYPE_2_05, 132, 0, 163, 32);
	InsertSpriteExplosion(EXPLOSION_SPRITE_ID::TYPE_2_06, 296, 0, 327, 32);


#pragma endregion Load Sprites

	OutputDebugString(L"Explosion Sprites Loaded Successfully\n");
}

void Explosion::LoadTextures()
{
	if (HasTextures<Explosion>::_hasBeenLoaded) return;
	HasTextures<Explosion>::_hasBeenLoaded = true;

	GraphicsHelper::InsertTexture(EXPLOSION_TEXTURE_ID::EXPLOSION, L"Resources\\Textures\\Explosion.png");

	OutputDebugString(L"Explosion Textures Loaded Successfully\n");
}

void Explosion::LoadAnimations()
{
	if (HasAnimations<Explosion>::_hasBeenLoaded) return;
	HasAnimations<Explosion>::_hasBeenLoaded = true;

#pragma region Load Animations

	GraphicsHelper::InsertAnimation(EXPLOSION_ANIMATION_ID::TYPE_1, Constants::Objects::Explosion::ANIMATION_DELAY_MILLISECONDS,
		{
			{EXPLOSION_SPRITE_ID::TYPE_1_01,0},
			{EXPLOSION_SPRITE_ID::TYPE_1_02,0},
			{EXPLOSION_SPRITE_ID::TYPE_1_03,0},
			{EXPLOSION_SPRITE_ID::TYPE_1_04,0},
			{EXPLOSION_SPRITE_ID::TYPE_1_05,0},
			{EXPLOSION_SPRITE_ID::TYPE_1_06,0},
		});
	GraphicsHelper::InsertAnimation(EXPLOSION_ANIMATION_ID::TYPE_2, Constants::Objects::Explosion::ANIMATION_DELAY_MILLISECONDS,
		{
			{EXPLOSION_SPRITE_ID::TYPE_2_01,0},
			{EXPLOSION_SPRITE_ID::TYPE_2_02,0},
			{EXPLOSION_SPRITE_ID::TYPE_2_03,0},
			{EXPLOSION_SPRITE_ID::TYPE_2_04,0},
			{EXPLOSION_SPRITE_ID::TYPE_2_05,0},
			{EXPLOSION_SPRITE_ID::TYPE_2_06,0},
		});
	GraphicsHelper::InsertAnimation(EXPLOSION_ANIMATION_ID::TYPE_3, Constants::Objects::Explosion::ANIMATION_DELAY_MILLISECONDS,
		{
			{EXPLOSION_SPRITE_ID::TYPE_2_01,0},
			{EXPLOSION_SPRITE_ID::TYPE_2_03,0},
			{EXPLOSION_SPRITE_ID::TYPE_2_05,0},
			{EXPLOSION_SPRITE_ID::TYPE_2_03,0},
			{EXPLOSION_SPRITE_ID::TYPE_2_01,0},
		});
#pragma endregion Load Animations

	OutputDebugString(L"Explosion Animations Loaded Successfully\n");
}
