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

	// set direction default is right
	this->movingDirection = DIRECTION::RIGHT;

	this->_ammoType = type;
	this->_aircarftDirection = direction;

	// y0: vị trí lúc đầu
	// time: thời gian
	// dt: delta time ( t = t + dt )
	// T: khoảng thời gian để quay hết 1 vòng (tính bằng giây)
	// A: Bán kính
	// φ: pha ban đầu của dao động (-π<φ<π)

	x0 = NULL;
	y0 = NULL;
	time = 0.0f;
	dt = 0.03f;
	T = 2.5f;
	A = 35.0f;
	φ = 0.0f;

	this->hitCounts = 1;
	this->enemyType = ENEMY_TYPE::MACHINE;
}

AirCraft::~AirCraft()
{
	x0 = NULL;
	y0 = NULL;
	time = NULL;
	dt = NULL;
	T = NULL;
	A = NULL;
	φ = NULL;

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
	if (isDead) Sound::getInstance()->play("qexplode", false, 1);
	// Chuyen dong hinh sin
	FLOAT x = GetX();
	FLOAT y = GetY();
	FLOAT vx = GetVX();
	FLOAT vy = GetVY();

	// gán vị trí lúc đầu
	if (x0 == NULL) x0 = x;
	if (y0 == NULL) y0 = y;

	if (getAircarftDirection() == AIRCRAFT_DIRECTION::HORIZONTAL) {
		// di chuyen theo chieu ngang
		Motion::OscillatoryMotionInputParameters pio{ y0, time, dt, T, A, φ };
		auto poo = Motion::CalculateOscillatoryMotion(pio);
		time = poo.t;

		x += vx;

		SetX(x);
		SetY(poo.c);
	}
	else
	{
		// di chuyen theo chieu doc
		Motion::OscillatoryMotionInputParameters pio{ x0, time, dt, T, A, φ };
		auto poo = Motion::CalculateOscillatoryMotion(pio);
		time = poo.t;

		y += vy;

		SetY(y);
		SetX(poo.c);
	}

}

void AirCraft::Render()
{
	this->w = this->currentFrameW;
	this->h = this->currentFrameH;
	SetAnimation(AIRCRAFT_ANIMATION_ID::NORMAL, GetPosition(), GetMovingDirection(), GetAngle());
}

void AirCraft::HandleInput(Input& input)
{
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

#pragma endregion Load Animations

	OutputDebugString(L"AirCraft Animations Loaded Successfully\n");
}