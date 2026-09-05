#include "AirCraft.h"
#include "Item.h"

AirCraft::AirCraft(ITEM_TYPE ammoType, AIRCRAFT_DIRECTION direction) : Entity(), HasTextures(), HasSprites(), HasAnimations()
{

	this->_vx = 1.0f;
	this->_vy = Constants::Enemies::AirCraft::INITIAL_SPEED_Y;
	this->_ax = Constants::Physics::DEFAULT_INITIAL_ACCELERATION_X;
	this->_ay = Constants::Physics::DEFAULT_INITIAL_ACCELERATION_Y;
	this->_position.x = Constants::Enemies::AirCraft::DEFAULT_SPAWN_X;
	this->_position.y = Constants::Enemies::AirCraft::DEFAULT_SPAWN_Y;
	this->SetDebugName(L"AirCraft\n");

	// set direction default is right
	this->_movingDirection = DIRECTION::RIGHT;

	this->_ammoType = ammoType;
	this->_aircraftDirection = direction;

	// y0: initial position
	// time: time
	// dt: delta time ( t = t + dt )
	// period: period (in seconds)
	// amplitude: amplitude / radius
	// phi: initial phase (-pi < phi < pi)

	this->_x0 = 0.0f;
	this->_y0 = 0.0f;
	this->_hasCapturedOrigin = false;
	this->_time = 0.0f;
	this->_dt = Constants::Enemies::AirCraft::OSCILLATION_DELTA_TIME;
	this->_period = Constants::Enemies::AirCraft::OSCILLATION_PERIOD;
	this->_amplitude = Constants::Enemies::AirCraft::OSCILLATION_RADIUS;
	this->_phi = 0.0f;

	this->_hitCounts = Constants::Enemies::AirCraft::HEALTH_POINTS;
	this->_enemyType = ENEMY_TYPE::MACHINE;
}

AirCraft::~AirCraft()
{
}

void AirCraft::SetAmmoType(ITEM_TYPE ammoType)
{
	this->_ammoType = ammoType;
}

ITEM_TYPE AirCraft::GetAmmoType()
{
	return this->_ammoType;
}

Item* AirCraft::CreateDroppedItem() const
{
	Item* item = new Item(this->_ammoType);
	item->SetX(this->GetX());
	item->SetY(this->GetY());
	return item;
}

void AirCraft::Update()
{
	// Sinusoidal motion
	float x = this->GetX();
	float y = this->GetY();
	float vx = this->GetVX();
	float vy = this->GetVY();

	// Capture the oscillation centre once, on the first update.  Keying this off
	// "_x0 == 0.0f" treated a craft that genuinely spawned at 0 as uncaptured,
	// so frame 1 stored 0 and moved it to 0 + A, and frame 2 - still seeing 0 -
	// re-captured that displaced position, shifting the centre by a full
	// amplitude for the rest of its life.
	if (!this->_hasCapturedOrigin)
	{
		this->_x0 = x;
		this->_y0 = y;
		this->_hasCapturedOrigin = true;
	}

	if (this->GetAircraftDirection() == AIRCRAFT_DIRECTION::HORIZONTAL) {
		// Horizontal oscillation
		Motion::OscillatoryMotionInputParameters pio{ this->_y0, this->_time, this->_dt, this->_period, this->_amplitude, this->_phi };
		auto poo = Motion::CalculateOscillatoryMotion(pio);
		this->_time = poo.elapsedTime;

		x += vx;

		this->SetX(x);
		this->SetY(poo.coordinate);
	}
	else
	{
		// Vertical oscillation
		Motion::OscillatoryMotionInputParameters pio{ this->_x0, this->_time, this->_dt, this->_period, this->_amplitude, this->_phi };
		auto poo = Motion::CalculateOscillatoryMotion(pio);
		this->_time = poo.elapsedTime;

		y += vy;

		this->SetY(y);
		this->SetX(poo.coordinate);
	}

}

void AirCraft::Render()
{
	this->_w = this->GetCurrentFrameW();
	this->_h = this->GetCurrentFrameH();
	this->SetAnimation(AIRCRAFT_ANIMATION_ID::NORMAL, this->GetPosition(), this->GetMovingDirection(), this->GetAngle());
}

void AirCraft::HandleInput(Input& input)
{
}

void InsertSpriteAirCraft(SPRITE_ID spriteId, int left, int top, int right, int bottom)
{
	// i write this function to shorten the function: GraphicsHelper
	GraphicsHelper::InsertSprite(spriteId, top, left, right, bottom, DIRECTION::RIGHT, AIRCRAFT_TEXTURE_ID::AIRCRAFT_01);
}

void AirCraft::LoadSprites()
{
	if (HasSprites<AirCraft>::_hasBeenLoaded) return;
	HasSprites<AirCraft>::_hasBeenLoaded = true;

#pragma region Load Sprites

	// SPRITES

	InsertSpriteAirCraft(AIRCRAFT_SPRITE_ID::NORMAL_01, 2, 0, 27, 15);

#pragma endregion Load Sprites

	OutputDebugString(L"AirCraft Sprites Loaded Successfully\n");
}

void AirCraft::LoadTextures()
{
	if (HasTextures<AirCraft>::_hasBeenLoaded) return;
	HasTextures<AirCraft>::_hasBeenLoaded = true;

	GraphicsHelper::InsertTexture(AIRCRAFT_TEXTURE_ID::AIRCRAFT_01, L"Resources\\Textures\\Aircraft.bmp");

	OutputDebugString(L"AirCraft Textures Loaded Successfully\n");
}

void AirCraft::LoadAnimations()
{
	if (HasAnimations<AirCraft>::_hasBeenLoaded) return;
	HasAnimations<AirCraft>::_hasBeenLoaded = true;

#pragma region Load Animations

	GraphicsHelper::InsertAnimation(AIRCRAFT_ANIMATION_ID::NORMAL, Constants::Enemies::AirCraft::ANIMATION_DELAY_MILLISECONDS,
		{
			{AIRCRAFT_SPRITE_ID::NORMAL_01,0},
		});

#pragma endregion Load Animations

	OutputDebugString(L"AirCraft Animations Loaded Successfully\n");
}
