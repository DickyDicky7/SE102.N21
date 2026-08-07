#include "Explosion.h"
// O: open
// P: close

Explosion::Explosion() : Entity(), HasTextures(), HasSprites(), HasAnimations()
{
	this->vx = 1.0f;
	this->vy = 1.0f;
	this->ax = 0.1f;
	this->ay = 0.1f;
	this->position.x = 200;
	this->position.y = 200;
	this->name = L"Explosion\n";

	this->updateState = NULL;
	this->handleInputState = NULL;
	// set direction default is right
	this->movingDirection = DIRECTION::RIGHT;
	// set state begin is run
	//this->state = new ExplosionType3State();
}

Explosion::Explosion(ExplosionState* explosionState) : Explosion()
{
	Destroy(this->state);
	this->state = explosionState;
}

Explosion::~Explosion()
{
	Destroy(state);
	Destroy(updateState);
	Destroy(handleInputState);
}

void Explosion::Update()
{
	updateState = state->Update(*this);
}

void Explosion::Render()
{
	state->Render(*this);
	this->w = this->currentFrameW;
	this->h = this->currentFrameH;

	if (updateState)
	{
		ChangeState(state, updateState, this);
		updateState = NULL;
	}
	if (handleInputState)
	{
		ChangeState(state, handleInputState, this);
		handleInputState = NULL;
	}
}

void Explosion::HandleInput(Input& input)
{
	handleInputState = state->HandleInput(*this, input);
}

void InsertSpriteExplosion(SPRITE_ID spriteId, INT left, INT top, INT right, INT bottom)
{
	// i write this function to shorten the fuction: GraphicsHelper
	GraphicsHelper::InsertSprite(spriteId, top, left, right, bottom, DIRECTION::RIGHT, EXPLOSION_TEXTURE_ID::EXPLOSION);
}

void Explosion::LoadSprites()
{
	if (HasSprites<Explosion>::hasBeenLoaded.value) return;
	HasSprites<Explosion>::hasBeenLoaded.value = true;

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
	if (HasTextures<Explosion>::hasBeenLoaded.value) return;
	HasTextures<Explosion>::hasBeenLoaded.value = true;

	GraphicsHelper::InsertTexure(EXPLOSION_TEXTURE_ID::EXPLOSION, L"Resources\\Textures\\Explosion.png");

	OutputDebugString(L"Explosion Textures Loaded Successfully\n");
}

void Explosion::LoadAnimations()
{
	if (HasAnimations<Explosion>::hasBeenLoaded.value) return;
	HasAnimations<Explosion>::hasBeenLoaded.value = true;

#pragma region Load Animations

	GraphicsHelper::InsertAnimation(EXPLOSION_ANIMATION_ID::TYPE_1, 100,
		{
			{EXPLOSION_SPRITE_ID::TYPE_1_01,0},
			{EXPLOSION_SPRITE_ID::TYPE_1_02,0},
			{EXPLOSION_SPRITE_ID::TYPE_1_03,0},
			{EXPLOSION_SPRITE_ID::TYPE_1_04,0},
			{EXPLOSION_SPRITE_ID::TYPE_1_05,0},
			{EXPLOSION_SPRITE_ID::TYPE_1_06,0},
		});
	GraphicsHelper::InsertAnimation(EXPLOSION_ANIMATION_ID::TYPE_2, 100,
		{
			{EXPLOSION_SPRITE_ID::TYPE_2_01,0},
			{EXPLOSION_SPRITE_ID::TYPE_2_02,0},
			{EXPLOSION_SPRITE_ID::TYPE_2_03,0},
			{EXPLOSION_SPRITE_ID::TYPE_2_04,0},
			{EXPLOSION_SPRITE_ID::TYPE_2_05,0},
			{EXPLOSION_SPRITE_ID::TYPE_2_06,0},
		});
	GraphicsHelper::InsertAnimation(EXPLOSION_ANIMATION_ID::TYPE_3, 100,
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
