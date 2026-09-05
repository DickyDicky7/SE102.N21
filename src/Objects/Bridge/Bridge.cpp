#include <cmath>
#include "Bridge.h"

Bridge::Bridge()
{
	this->_vx = Constants::Physics::DEFAULT_INITIAL_VELOCITY_X;
	this->_vy = Constants::Physics::DEFAULT_INITIAL_VELOCITY_Y;
	this->_ax = Constants::Physics::DEFAULT_INITIAL_ACCELERATION_X;
	this->_ay = Constants::Physics::DEFAULT_INITIAL_ACCELERATION_Y;
	this->_angle = 0;
	this->_position.x = Constants::Enemies::Bridge::DEFAULT_SPAWN_X;
	this->_position.y = Constants::Enemies::Bridge::DEFAULT_SPAWN_Y;
	this->_w = 0.0f;
	this->_h = 0.0f;

	//
	this->SetDebugName(L"Bridge\n");
	//
	this->_isInitBridge = false;
	this->InitBridgePart();
}

Bridge::Bridge(D3DXVECTOR3 position, float w)
{
	this->_vx = Constants::Physics::DEFAULT_INITIAL_VELOCITY_X;
	this->_vy = Constants::Physics::DEFAULT_INITIAL_VELOCITY_Y;
	this->_ax = Constants::Physics::DEFAULT_INITIAL_ACCELERATION_X;
	this->_ay = Constants::Physics::DEFAULT_INITIAL_ACCELERATION_Y;

	this->_position.x = position.x;
	this->_position.y = position.y;

	this->_w = w;
	this->_h = w / Constants::Enemies::Bridge::PART_COUNT;

	//
	this->SetDebugName(L"Bridge\n");
	//
	this->_isInitBridge = false;
	this->InitBridgePart();
}

Bridge::~Bridge()
{
	for (int i = 0; i < Constants::Enemies::Bridge::PART_COUNT; i++)
	{
		Destroy(this->_bridgePart[i]);
	}
}

void Bridge::InitBridgePart()
{
	float bridgePartW = this->_w / Constants::Enemies::Bridge::PART_COUNT;

	for (int i = 0; i < Constants::Enemies::Bridge::PART_COUNT; i++)
	{
		D3DXVECTOR3 partPosition = this->_position;
		partPosition.x = this->_position.x - this->_w * 0.5f + bridgePartW * (0.5f + i);

		BRIDGE_ANIMATION_ID animId = (i == 0) ? BRIDGE_ANIMATION_ID::HEAD
			: (i == Constants::Enemies::Bridge::PART_COUNT - 1) ? BRIDGE_ANIMATION_ID::TAIL
			: BRIDGE_ANIMATION_ID::BODY;

		this->_bridgePart[i] = new BridgePart(animId);
		this->_bridgePart[i]->SetW(bridgePartW);
		this->_bridgePart[i]->SetH(bridgePartW);
		this->_bridgePart[i]->SetX(partPosition.x);
		this->_bridgePart[i]->SetY(partPosition.y);
	}

	this->_isInitBridge = true;
}

void Bridge::Update()
{
	// overcome PART_COUNT then dont do anything
	if (this->_lastDestroyPart >= Constants::Enemies::Bridge::PART_COUNT)
	{
		return;
	}

	bool isEnemyTouch = this->_target && std::floor(this->_target->GetX() + this->_target->GetW() * Constants::Enemies::Bridge::ENEMY_TOUCH_TRIGGER_OFFSET_RATIO) >= std::floor(this->GetX() - this->_w * 0.5f);

	// if bill touch bridge or is destroying
	if (isEnemyTouch || this->_lastDestroyPart != -1)
	{
		// first part
		if (this->_lastDestroyPart == -1)
		{
			this->_bridgePart[++this->_lastDestroyPart]->Update();
			return;
		}

		// if current part is destroy
		if (this->_bridgePart[this->_lastDestroyPart]->GetIsDestroy())
		{
			Destroy(this->_bridgePart[this->_lastDestroyPart]);

			// if increase index not larger then maximum part number
			if (this->_lastDestroyPart + 1 < Constants::Enemies::Bridge::PART_COUNT)
			{
				this->_bridgePart[++this->_lastDestroyPart]->Update();
				return;
			}
			// else increase to first if come true
			++this->_lastDestroyPart;
			return;
		}

		// if current not destroy, then continue update
		this->_bridgePart[this->_lastDestroyPart]->Update();
		return;
	}
}


void Bridge::Render()
{
	float ow = this->_w;
	this->_w = 0.0f;
	this->_h = 0.0f;
	for (int i = 0; i < Constants::Enemies::Bridge::PART_COUNT; i++)
	{
		if (this->_bridgePart[i])
		{
			this->_bridgePart[i]->Render();
			this->_w += this->_bridgePart[i]->GetW() * Constants::Enemies::Bridge::HITBOX_WIDTH_RATIO_PER_PART;
			this->_h = this->_bridgePart[i]->GetH() * Constants::Enemies::Bridge::HITBOX_HEIGHT_RATIO;
		}
	}
	float L = this->GetL() + ow - this->_w;
	this->_position.x = L + this->_w * 0.5f;
}

void Bridge::HandleInput(Input&)
{

}

void Bridge::LoadSprites()
{
	if (HasSprites::_hasBeenLoaded)
	{
		return;
	}

	HasSprites::_hasBeenLoaded = true;

	GraphicsHelper::InsertSprite(BRIDGE_SPRITE_ID::BRIDGE_HEAD_00, 0, 0, 32, 32, DIRECTION::LEFT, BRIDGE_TEXTURE_ID::BRIDGE_HEAD_00);
	GraphicsHelper::InsertSprite(BRIDGE_SPRITE_ID::BRIDGE_HEAD_01, 0, 0, 32, 32, DIRECTION::LEFT, BRIDGE_TEXTURE_ID::BRIDGE_HEAD_01);
	GraphicsHelper::InsertSprite(BRIDGE_SPRITE_ID::BRIDGE_HEAD_02, 0, 0, 32, 32, DIRECTION::LEFT, BRIDGE_TEXTURE_ID::BRIDGE_HEAD_02);

	GraphicsHelper::InsertSprite(BRIDGE_SPRITE_ID::BRIDGE_BODY_00, 0, 0, 32, 32, DIRECTION::LEFT, BRIDGE_TEXTURE_ID::BRIDGE_BODY_00);
	GraphicsHelper::InsertSprite(BRIDGE_SPRITE_ID::BRIDGE_BODY_01, 0, 0, 32, 32, DIRECTION::LEFT, BRIDGE_TEXTURE_ID::BRIDGE_BODY_01);
	GraphicsHelper::InsertSprite(BRIDGE_SPRITE_ID::BRIDGE_BODY_02, 0, 0, 32, 32, DIRECTION::LEFT, BRIDGE_TEXTURE_ID::BRIDGE_BODY_02);

	GraphicsHelper::InsertSprite(BRIDGE_SPRITE_ID::BRIDGE_TAIL_00, 0, 0, 32, 32, DIRECTION::LEFT, BRIDGE_TEXTURE_ID::BRIDGE_TAIL_00);
	GraphicsHelper::InsertSprite(BRIDGE_SPRITE_ID::BRIDGE_TAIL_01, 0, 0, 32, 32, DIRECTION::LEFT, BRIDGE_TEXTURE_ID::BRIDGE_TAIL_01);
	GraphicsHelper::InsertSprite(BRIDGE_SPRITE_ID::BRIDGE_TAIL_02, 0, 0, 32, 32, DIRECTION::LEFT, BRIDGE_TEXTURE_ID::BRIDGE_TAIL_02);

	GraphicsHelper::InsertSprite(BRIDGE_SPRITE_ID::BRIDGE_EXPLOSION_00, 0, 0, 62, 46, DIRECTION::LEFT, BRIDGE_TEXTURE_ID::BRIDGE_EXPLOSION_00);
	GraphicsHelper::InsertSprite(BRIDGE_SPRITE_ID::BRIDGE_EXPLOSION_01, 0, 0, 62, 46, DIRECTION::LEFT, BRIDGE_TEXTURE_ID::BRIDGE_EXPLOSION_01);
	GraphicsHelper::InsertSprite(BRIDGE_SPRITE_ID::BRIDGE_EXPLOSION_02, 0, 0, 62, 46, DIRECTION::LEFT, BRIDGE_TEXTURE_ID::BRIDGE_EXPLOSION_02);
	GraphicsHelper::InsertSprite(BRIDGE_SPRITE_ID::BRIDGE_EXPLOSION_03, 0, 0, 62, 46, DIRECTION::LEFT, BRIDGE_TEXTURE_ID::BRIDGE_EXPLOSION_03);
	GraphicsHelper::InsertSprite(BRIDGE_SPRITE_ID::BRIDGE_EXPLOSION_04, 0, 0, 62, 46, DIRECTION::LEFT, BRIDGE_TEXTURE_ID::BRIDGE_EXPLOSION_04);
	GraphicsHelper::InsertSprite(BRIDGE_SPRITE_ID::BRIDGE_EXPLOSION_05, 0, 0, 62, 46, DIRECTION::LEFT, BRIDGE_TEXTURE_ID::BRIDGE_EXPLOSION_05);
	GraphicsHelper::InsertSprite(BRIDGE_SPRITE_ID::BRIDGE_EXPLOSION_06, 0, 0, 62, 46, DIRECTION::LEFT, BRIDGE_TEXTURE_ID::BRIDGE_EXPLOSION_06);
	GraphicsHelper::InsertSprite(BRIDGE_SPRITE_ID::BRIDGE_EXPLOSION_07, 0, 0, 62, 46, DIRECTION::LEFT, BRIDGE_TEXTURE_ID::BRIDGE_EXPLOSION_07);
	GraphicsHelper::InsertSprite(BRIDGE_SPRITE_ID::BRIDGE_EXPLOSION_08, 0, 0, 62, 46, DIRECTION::LEFT, BRIDGE_TEXTURE_ID::BRIDGE_EXPLOSION_08);
	GraphicsHelper::InsertSprite(BRIDGE_SPRITE_ID::BRIDGE_EXPLOSION_09, 0, 0, 62, 46, DIRECTION::LEFT, BRIDGE_TEXTURE_ID::BRIDGE_EXPLOSION_09);
	GraphicsHelper::InsertSprite(BRIDGE_SPRITE_ID::BRIDGE_EXPLOSION_10, 0, 0, 62, 46, DIRECTION::LEFT, BRIDGE_TEXTURE_ID::BRIDGE_EXPLOSION_10);
	GraphicsHelper::InsertSprite(BRIDGE_SPRITE_ID::BRIDGE_EXPLOSION_11, 0, 0, 62, 46, DIRECTION::LEFT, BRIDGE_TEXTURE_ID::BRIDGE_EXPLOSION_11);
	GraphicsHelper::InsertSprite(BRIDGE_SPRITE_ID::BRIDGE_EXPLOSION_12, 0, 0, 62, 46, DIRECTION::LEFT, BRIDGE_TEXTURE_ID::BRIDGE_EXPLOSION_12);
	GraphicsHelper::InsertSprite(BRIDGE_SPRITE_ID::BRIDGE_EXPLOSION_13, 0, 0, 62, 46, DIRECTION::LEFT, BRIDGE_TEXTURE_ID::BRIDGE_EXPLOSION_13);
	GraphicsHelper::InsertSprite(BRIDGE_SPRITE_ID::BRIDGE_EXPLOSION_14, 0, 0, 62, 46, DIRECTION::LEFT, BRIDGE_TEXTURE_ID::BRIDGE_EXPLOSION_14);

	OutputDebugString(L"Bridge Sprites Loaded Successfully\n");
}

void Bridge::LoadTextures()
{
	if (HasTextures::_hasBeenLoaded)
	{
		return;
	}

	HasTextures::_hasBeenLoaded = true;

	GraphicsHelper::InsertTexture(BRIDGE_TEXTURE_ID::BRIDGE_BODY_00, L"Resources\\Textures\\bridge\\bridgebody00.bmp");
	GraphicsHelper::InsertTexture(BRIDGE_TEXTURE_ID::BRIDGE_BODY_01, L"Resources\\Textures\\bridge\\bridgebody01.bmp");
	GraphicsHelper::InsertTexture(BRIDGE_TEXTURE_ID::BRIDGE_BODY_02, L"Resources\\Textures\\bridge\\bridgebody02.bmp");

	GraphicsHelper::InsertTexture(BRIDGE_TEXTURE_ID::BRIDGE_HEAD_00, L"Resources\\Textures\\bridge\\bridgehead00.bmp");
	GraphicsHelper::InsertTexture(BRIDGE_TEXTURE_ID::BRIDGE_HEAD_01, L"Resources\\Textures\\bridge\\bridgehead01.bmp");
	GraphicsHelper::InsertTexture(BRIDGE_TEXTURE_ID::BRIDGE_HEAD_02, L"Resources\\Textures\\bridge\\bridgehead02.bmp");

	GraphicsHelper::InsertTexture(BRIDGE_TEXTURE_ID::BRIDGE_TAIL_00, L"Resources\\Textures\\bridge\\bridgetail00.bmp");
	GraphicsHelper::InsertTexture(BRIDGE_TEXTURE_ID::BRIDGE_TAIL_01, L"Resources\\Textures\\bridge\\bridgetail01.bmp");
	GraphicsHelper::InsertTexture(BRIDGE_TEXTURE_ID::BRIDGE_TAIL_02, L"Resources\\Textures\\bridge\\bridgetail02.bmp");

	GraphicsHelper::InsertTexture(BRIDGE_TEXTURE_ID::BRIDGE_EXPLOSION_00, L"Resources\\Textures\\bridge\\bridgeexplosion00.bmp");
	GraphicsHelper::InsertTexture(BRIDGE_TEXTURE_ID::BRIDGE_EXPLOSION_01, L"Resources\\Textures\\bridge\\bridgeexplosion01.bmp");
	GraphicsHelper::InsertTexture(BRIDGE_TEXTURE_ID::BRIDGE_EXPLOSION_02, L"Resources\\Textures\\bridge\\bridgeexplosion02.bmp");
	GraphicsHelper::InsertTexture(BRIDGE_TEXTURE_ID::BRIDGE_EXPLOSION_03, L"Resources\\Textures\\bridge\\bridgeexplosion03.bmp");
	GraphicsHelper::InsertTexture(BRIDGE_TEXTURE_ID::BRIDGE_EXPLOSION_04, L"Resources\\Textures\\bridge\\bridgeexplosion04.bmp");
	GraphicsHelper::InsertTexture(BRIDGE_TEXTURE_ID::BRIDGE_EXPLOSION_05, L"Resources\\Textures\\bridge\\bridgeexplosion05.bmp");
	GraphicsHelper::InsertTexture(BRIDGE_TEXTURE_ID::BRIDGE_EXPLOSION_06, L"Resources\\Textures\\bridge\\bridgeexplosion06.bmp");
	GraphicsHelper::InsertTexture(BRIDGE_TEXTURE_ID::BRIDGE_EXPLOSION_07, L"Resources\\Textures\\bridge\\bridgeexplosion07.bmp");
	GraphicsHelper::InsertTexture(BRIDGE_TEXTURE_ID::BRIDGE_EXPLOSION_08, L"Resources\\Textures\\bridge\\bridgeexplosion08.bmp");
	GraphicsHelper::InsertTexture(BRIDGE_TEXTURE_ID::BRIDGE_EXPLOSION_09, L"Resources\\Textures\\bridge\\bridgeexplosion09.bmp");
	GraphicsHelper::InsertTexture(BRIDGE_TEXTURE_ID::BRIDGE_EXPLOSION_10, L"Resources\\Textures\\bridge\\bridgeexplosion10.bmp");
	GraphicsHelper::InsertTexture(BRIDGE_TEXTURE_ID::BRIDGE_EXPLOSION_11, L"Resources\\Textures\\bridge\\bridgeexplosion11.bmp");
	GraphicsHelper::InsertTexture(BRIDGE_TEXTURE_ID::BRIDGE_EXPLOSION_12, L"Resources\\Textures\\bridge\\bridgeexplosion12.bmp");
	GraphicsHelper::InsertTexture(BRIDGE_TEXTURE_ID::BRIDGE_EXPLOSION_13, L"Resources\\Textures\\bridge\\bridgeexplosion13.bmp");
	GraphicsHelper::InsertTexture(BRIDGE_TEXTURE_ID::BRIDGE_EXPLOSION_14, L"Resources\\Textures\\bridge\\bridgeexplosion14.bmp");

	OutputDebugString(L"Bridge Texture Loaded Successfully\n");
}

void Bridge::LoadAnimations()
{
	if (HasAnimations::_hasBeenLoaded) {
		return;
	}
	HasAnimations::_hasBeenLoaded = true;

	GraphicsHelper::InsertAnimation(BRIDGE_ANIMATION_ID::BODY, Constants::Enemies::Bridge::ANIMATION_DEFAULT_DELAY_MILLISECONDS,
		{
			{BRIDGE_SPRITE_ID::BRIDGE_BODY_00, 0},
			{BRIDGE_SPRITE_ID::BRIDGE_BODY_01, 0},
			{BRIDGE_SPRITE_ID::BRIDGE_BODY_02, 0},
		});

	GraphicsHelper::InsertAnimation(BRIDGE_ANIMATION_ID::HEAD, Constants::Enemies::Bridge::ANIMATION_DEFAULT_DELAY_MILLISECONDS,
		{
			{BRIDGE_SPRITE_ID::BRIDGE_HEAD_00, 0},
			{BRIDGE_SPRITE_ID::BRIDGE_HEAD_01, 0},
			{BRIDGE_SPRITE_ID::BRIDGE_HEAD_02, 0},
		});

	GraphicsHelper::InsertAnimation(BRIDGE_ANIMATION_ID::TAIL, Constants::Enemies::Bridge::ANIMATION_DEFAULT_DELAY_MILLISECONDS,
		{
			{BRIDGE_SPRITE_ID::BRIDGE_TAIL_00, 0},
			{BRIDGE_SPRITE_ID::BRIDGE_TAIL_01, 0},
			{BRIDGE_SPRITE_ID::BRIDGE_TAIL_02, 0},
		});

	GraphicsHelper::InsertAnimation(BRIDGE_ANIMATION_ID::EXPLOSION, Constants::Enemies::Bridge::ANIMATION_EXPLOSION_DELAY_MILLISECONDS,
		{
			{BRIDGE_SPRITE_ID::BRIDGE_EXPLOSION_00, 0},
			{BRIDGE_SPRITE_ID::BRIDGE_EXPLOSION_01, 0},
			{BRIDGE_SPRITE_ID::BRIDGE_EXPLOSION_02, 0},
			{BRIDGE_SPRITE_ID::BRIDGE_EXPLOSION_03, 0},
			{BRIDGE_SPRITE_ID::BRIDGE_EXPLOSION_04, 0},
			{BRIDGE_SPRITE_ID::BRIDGE_EXPLOSION_05, 0},
			{BRIDGE_SPRITE_ID::BRIDGE_EXPLOSION_06, 0},
			{BRIDGE_SPRITE_ID::BRIDGE_EXPLOSION_07, 0},
			{BRIDGE_SPRITE_ID::BRIDGE_EXPLOSION_08, 0},
			{BRIDGE_SPRITE_ID::BRIDGE_EXPLOSION_09, 0},
			{BRIDGE_SPRITE_ID::BRIDGE_EXPLOSION_10, 0},
			{BRIDGE_SPRITE_ID::BRIDGE_EXPLOSION_11, 0},
			{BRIDGE_SPRITE_ID::BRIDGE_EXPLOSION_12, 0},
			{BRIDGE_SPRITE_ID::BRIDGE_EXPLOSION_13, 0},
			{BRIDGE_SPRITE_ID::BRIDGE_EXPLOSION_14, 0},
		});

	OutputDebugString(L"Bridge Animation Loaded Successfully\n");
}
