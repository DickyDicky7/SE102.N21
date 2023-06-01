#include "Cannon.h"

Cannon::Cannon()
{
	this->vx = 1.0f;
	this->vy = 1.0f;
	this->ax = 0.1f;
	this->ay = 0.1f;
	this->angle = 0;
	this->position.x = 50;
	this->position.y = 00;

	this->state = NULL;
	this->updateState = NULL;

	//
	this->name = L"Cannon\n";
	//
}

Cannon::~Cannon()
{
	Destroy(state);
	Destroy(updateState);
}

BOOLEAN Cannon::IsTargetInRange()
{
	float dx = this->GetX() - Enemy::target->GetX();
	float dy = this->GetY() - Enemy::target->GetY();

	return (dx > 0 && dy <= 0 && CalculateShootingAngle() > 0);
}

const Bill* Cannon::GetEnemyTarget()
{
	return Enemy::target;
}

FLOAT Cannon::CalculateShootingAngle()
{
	// first and third quarter is positive, second and fourth quarter is negative, top and bottom is 0, left right is 90
	const Bill* bill = Enemy::target;

	float dx = (this->GetPosition().x) - (bill->GetPosition().x);
	float dy = -((this->GetPosition().y) - (bill->GetPosition().y));

	if (dy == 0)
	{
		return 90;
	}

	return D3DXToDegree(atan(dx / dy));
}

void Cannon::Update()
{
	if (!state)
	{
		state = new CannonAppearState();
	}

	updateState = state->Update(*this);
}

void Cannon::Render()
{
	state->Render(*this);
	this->w = this->currentFrameW;
	this->h = this->currentFrameH;

	if (updateState)
	{
		ChangeState(state, updateState, this);
		updateState = NULL;
	}
}

void Cannon::HandleInput(Input&)
{

}

void Cannon::LoadTextures()
{
	if (HasTextures<Cannon>::hasBeenLoaded.value) {
		return;
	}
	HasTextures<Cannon>::hasBeenLoaded.value = true;

	GraphicsHelper::InsertTexure(CANNON_TEXTURE_ID::CANNON, L"Resources\\Textures\\Cannon_all.bmp");
}

void Cannon::LoadSprites()
{
	if (HasSprites<Cannon>::hasBeenLoaded.value) {
		return;
	}
	HasSprites<Cannon>::hasBeenLoaded.value = true;

	GraphicsHelper::InsertSprite(CANNON_SPRITE_ID::HALF_APPEAR_01, 2, 2, 34, 34, DIRECTION::LEFT, CANNON_TEXTURE_ID::CANNON);
	GraphicsHelper::InsertSprite(CANNON_SPRITE_ID::HALF_APPEAR_02, 34, 2, 34, 66, DIRECTION::LEFT, CANNON_TEXTURE_ID::CANNON);
	GraphicsHelper::InsertSprite(CANNON_SPRITE_ID::HALF_APPEAR_03, 66, 2, 34, 98, DIRECTION::LEFT, CANNON_TEXTURE_ID::CANNON);

	GraphicsHelper::InsertSprite(CANNON_SPRITE_ID::THREE_FOURS_APPEAR_01, 2, 34, 66, 34, DIRECTION::LEFT, CANNON_TEXTURE_ID::CANNON);
	GraphicsHelper::InsertSprite(CANNON_SPRITE_ID::THREE_FOURS_APPEAR_02, 34, 34, 66, 66, DIRECTION::LEFT, CANNON_TEXTURE_ID::CANNON);
	GraphicsHelper::InsertSprite(CANNON_SPRITE_ID::THREE_FOURS_APPEAR_03, 66, 34, 66, 98, DIRECTION::LEFT, CANNON_TEXTURE_ID::CANNON);

	GraphicsHelper::InsertSprite(CANNON_SPRITE_ID::NORMAL_01, 2, 66, 98, 34, DIRECTION::LEFT, CANNON_TEXTURE_ID::CANNON);
	GraphicsHelper::InsertSprite(CANNON_SPRITE_ID::NORMAL_02, 34, 66, 98, 66, DIRECTION::LEFT, CANNON_TEXTURE_ID::CANNON);
	GraphicsHelper::InsertSprite(CANNON_SPRITE_ID::NORMAL_03, 66, 66, 98, 98, DIRECTION::LEFT, CANNON_TEXTURE_ID::CANNON);

	GraphicsHelper::InsertSprite(CANNON_SPRITE_ID::UP_30_01, 2, 98, 130, 34, DIRECTION::LEFT, CANNON_TEXTURE_ID::CANNON);
	GraphicsHelper::InsertSprite(CANNON_SPRITE_ID::UP_30_02, 34, 98, 130, 66, DIRECTION::LEFT, CANNON_TEXTURE_ID::CANNON);
	GraphicsHelper::InsertSprite(CANNON_SPRITE_ID::UP_30_03, 66, 98, 130, 98, DIRECTION::LEFT, CANNON_TEXTURE_ID::CANNON);

	GraphicsHelper::InsertSprite(CANNON_SPRITE_ID::UP_60_01, 2, 130, 162, 34, DIRECTION::LEFT, CANNON_TEXTURE_ID::CANNON);
	GraphicsHelper::InsertSprite(CANNON_SPRITE_ID::UP_60_02, 34, 130, 162, 66, DIRECTION::LEFT, CANNON_TEXTURE_ID::CANNON);
	GraphicsHelper::InsertSprite(CANNON_SPRITE_ID::UP_60_03, 66, 130, 162, 98, DIRECTION::LEFT, CANNON_TEXTURE_ID::CANNON);
}

void Cannon::LoadAnimations()
{
	if (HasAnimations<Cannon>::hasBeenLoaded.value) {
		return;
	}
	HasAnimations<Cannon>::hasBeenLoaded.value = true;

	GraphicsHelper::InsertAnimation(CANNON_ANIMATION_ID::APPEAR, 50,
		{
			{CANNON_SPRITE_ID::HALF_APPEAR_01, 0},
			{CANNON_SPRITE_ID::HALF_APPEAR_02, 0},
			{CANNON_SPRITE_ID::HALF_APPEAR_03, 0},
			{CANNON_SPRITE_ID::THREE_FOURS_APPEAR_01, 0},
			{CANNON_SPRITE_ID::THREE_FOURS_APPEAR_02, 0},
			{CANNON_SPRITE_ID::THREE_FOURS_APPEAR_03, 0}
		});

	GraphicsHelper::InsertAnimation(CANNON_ANIMATION_ID::NORMAL, 165,
		{
			{CANNON_SPRITE_ID::NORMAL_01, 0},
			{CANNON_SPRITE_ID::NORMAL_02, 0},
			{CANNON_SPRITE_ID::NORMAL_03, 0}
		});

	GraphicsHelper::InsertAnimation(CANNON_ANIMATION_ID::UP_30, 165,
		{
			{CANNON_SPRITE_ID::UP_30_01, 0},
			{CANNON_SPRITE_ID::UP_30_02, 0},
			{CANNON_SPRITE_ID::UP_30_03, 0}
		});

	GraphicsHelper::InsertAnimation(CANNON_ANIMATION_ID::UP_60, 165,
		{
			{CANNON_SPRITE_ID::UP_60_01, 0},
			{CANNON_SPRITE_ID::UP_60_02, 0},
			{CANNON_SPRITE_ID::UP_60_03, 0}
		});
}

