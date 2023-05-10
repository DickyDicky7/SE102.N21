#include "Bullet.h"

Bullet::Bullet(                  ) : Entity(), HasTextures(), HasSprites(), HasAnimations()
{
	Entity::self = this;

	this->state = NULL;
	this->updateState = NULL;
	this->handleInputState = NULL;
}

Bullet::Bullet(BulletState* state) : Bullet()
{
	this->state = state;
}

Bullet::~Bullet()
{
	Destroy(state);
	Destroy(updateState);
	Destroy(handleInputState);
}

void Bullet::Update()
{
	updateState = state->Update(*this);
}

void Bullet::Render()
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

void Bullet::HandleInput(Input& input)
{
	handleInputState = state->HandleInput(*this, input);
}

void Bullet::LoadSprites()
{
	if (HasSprites<Bullet>::hasBeenLoaded.value) return;
	HasSprites<Bullet>::hasBeenLoaded.value = true;

	//GraphicsHelper::InsertSprite(BULLET_SPRITE_ID::R_01, 42, 58, 61, 45, DIRECTION::LEFT, BULLET_TEXTURE_ID::BULLET_01);
	GraphicsHelper::InsertSprite(BULLET_SPRITE_ID::R_01, 42, 52, 55, 45, DIRECTION::LEFT, BULLET_TEXTURE_ID::BULLET_01);
	GraphicsHelper::InsertSprite(BULLET_SPRITE_ID::M_01, 28, 69, 74, 33, DIRECTION::LEFT, BULLET_TEXTURE_ID::BULLET_01);
	GraphicsHelper::InsertSprite(BULLET_SPRITE_ID::S_01, 28, 69, 74, 33, DIRECTION::LEFT, BULLET_TEXTURE_ID::BULLET_01);
	GraphicsHelper::InsertSprite(BULLET_SPRITE_ID::S_02, 27, 61, 67, 33, DIRECTION::LEFT, BULLET_TEXTURE_ID::BULLET_01);
	GraphicsHelper::InsertSprite(BULLET_SPRITE_ID::S_03, 25, 50, 58, 33, DIRECTION::LEFT, BULLET_TEXTURE_ID::BULLET_01);
	GraphicsHelper::InsertSprite(BULLET_SPRITE_ID::L_01, 39, 23, 38, 45, DIRECTION::LEFT, BULLET_TEXTURE_ID::BULLET_01);
	GraphicsHelper::InsertSprite(BULLET_SPRITE_ID::F_01, 37, 41, 49, 45, DIRECTION::LEFT, BULLET_TEXTURE_ID::BULLET_01);

	OutputDebugString(L"Bullet Sprites Loaded Successfully\n");
}

void Bullet::LoadTextures()
{
	if (HasTextures<Bullet>::hasBeenLoaded.value) return;
	HasTextures<Bullet>::hasBeenLoaded.value = true;

	GraphicsHelper::InsertTexure(BULLET_TEXTURE_ID::BULLET_01, L"Resources\\Textures\\Bullet.bmp");

	OutputDebugString(L"Bullet Textures Loaded Successfully\n");
}

void Bullet::LoadAnimations()
{
	if (HasAnimations<Bullet>::hasBeenLoaded.value) return;
	HasAnimations<Bullet>::hasBeenLoaded.value = true;

	GraphicsHelper::InsertAnimation(BULLET_ANIMATION_ID::R, 150,
		{
			{BULLET_SPRITE_ID::R_01, 0},
		});

	GraphicsHelper::InsertAnimation(BULLET_ANIMATION_ID::M, 150,
		{
			{BULLET_SPRITE_ID::M_01, 0},
		});

	GraphicsHelper::InsertAnimation(BULLET_ANIMATION_ID::S, 500,
		{
			{BULLET_SPRITE_ID::S_01, 0},
			{BULLET_SPRITE_ID::S_02, 0},
			{BULLET_SPRITE_ID::S_03, 0},
		});

	GraphicsHelper::InsertAnimation(BULLET_ANIMATION_ID::L, 150,
		{
			{BULLET_SPRITE_ID::L_01, 0},
		});

	GraphicsHelper::InsertAnimation(BULLET_ANIMATION_ID::F, 150,
		{
			{BULLET_SPRITE_ID::F_01, 0},
		});

	OutputDebugString(L"Bullet Animations Loaded Successfully\n");
}

void Bullet::StaticResolveNoCollision(                               )
{
}

void Bullet::StaticResolveOnCollision(AABBSweepResult aabbSweepResult)
{
}

void Bullet::DynamicResolveNoCollision(                               )
{
}

void Bullet::DynamicResolveOnCollision(AABBSweepResult aabbSweepResult)
{
}
