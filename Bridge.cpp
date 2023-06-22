#include "Bridge.h"

Bridge::Bridge()
{
	this->vx = 1.0f;
	this->vy = 1.0f;
	this->ax = 0.1f;
	this->ay = 0.1f;
	this->angle = 0;
	this->position.x = 50;
	this->position.y = 00;

	//
	this->name = L"Bridge\n";
	//
	isInitBridge = false;
	InitBridgePart();
}

Bridge::Bridge(D3DXVECTOR3 _position, FLOAT _w)
{
	this->vx = 1.0f;
	this->vy = 1.0f;
	this->ax = 0.1f;
	this->ay = 0.1f;

	this->position.x = _position.x;
	this->position.y = _position.y;

	this->w = _w;
	this->h = _w / BRIDGE_PART;

	//
	this->name = L"Bridge\n";
	//
	isInitBridge = false;
	InitBridgePart();
}

Bridge::~Bridge()
{

}

void Bridge::InitBridgePart()
{
	FLOAT bridgePartW = w / BRIDGE_PART;

	for (int i = 0; i < BRIDGE_PART; i++)
	{
		D3DXVECTOR3 partPosition = position;
		partPosition.x = this->position.x - w * 0.5f + bridgePartW * (0.5f + i);

		switch (i)
		{
		case 0:
		{
			bridgePart[i] = new BridgePart(BRIDGE_ANIMATION_ID::HEAD);
			bridgePart[i]->SetW(bridgePartW);
			bridgePart[i]->SetH(bridgePartW);
			bridgePart[i]->SetX(partPosition.x);
			bridgePart[i]->SetY(partPosition.y);
		}
		break;
		case BRIDGE_PART - 1:
		{
			bridgePart[i] = new BridgePart(BRIDGE_ANIMATION_ID::TAIL);
			bridgePart[i]->SetW(bridgePartW);
			bridgePart[i]->SetH(bridgePartW);
			bridgePart[i]->SetX(partPosition.x);
			bridgePart[i]->SetY(partPosition.y);
		}
		break;
		default:
		{
			bridgePart[i] = new BridgePart(BRIDGE_ANIMATION_ID::BODY);
			bridgePart[i]->SetW(bridgePartW);
			bridgePart[i]->SetH(bridgePartW);
			bridgePart[i]->SetX(partPosition.x);
			bridgePart[i]->SetY(partPosition.y);
		}
		break;
		}
	}

	isInitBridge = true;
}

void Bridge::Update()
{
	// overcome BRIDGE_PART then dont do anything
	if (lastDestroyPart >= BRIDGE_PART)
	{
		return;
	}

	BOOLEAN isEnemyTouch = floor(Enemy::target->GetX() + Enemy::target->GetW() * 0.25f) >= floor(this->GetX() - w * 0.5f);

	OutputDebugString(L"update bridge\n");

	// if bill touch bridge or is destroying
	if (isEnemyTouch || lastDestroyPart != -1)
	{
		OutputDebugString(L"update bridge per part\n");
		// first part
		if (lastDestroyPart == -1)
		{
			bridgePart[++lastDestroyPart]->Update();
			return;
		}

		// if current part is destroy
		if (bridgePart[lastDestroyPart]->GetIsDestroy())
		{
			Destroy(bridgePart[lastDestroyPart]);

			// if increase index not larger then maximum part number
			if (lastDestroyPart + 1 < BRIDGE_PART)
			{
				bridgePart[++lastDestroyPart]->Update();
				return;
			}
			// else increase to first if come true
			++lastDestroyPart;
			return;
		}

		// if current not destroy, then continue update
		bridgePart[lastDestroyPart]->Update();
		return;
	}
}


void Bridge::Render()
{
	float ow = this->w;
	this->w = 0.0f;
	this->h = 0.0f;
	for (int i = 0; i < BRIDGE_PART; i++)
	{
		if (bridgePart[i])
		{
			bridgePart[i]->Render();
			this->w += bridgePart[i]->GetW() * 1.25f;
			this->h = bridgePart[i]->GetH() * 0.75f;
		}
	}
	float L = this->GetL() + ow - this->w;
	this->position.x = L + this->w * 0.5f;
}

void Bridge::HandleInput(Input&)
{

}

void Bridge::LoadSprites()
{
	if (HasSprites::hasBeenLoaded.value)
	{
		return;
	}

	HasSprites::hasBeenLoaded.value = true;

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
	if (HasTextures::hasBeenLoaded.value)
	{
		return;
	}

	HasTextures::hasBeenLoaded.value = true;

	GraphicsHelper::InsertTexure(BRIDGE_TEXTURE_ID::BRIDGE_BODY_00, L"Resources\\Textures\\bridge\\bridgebody00.bmp");
	GraphicsHelper::InsertTexure(BRIDGE_TEXTURE_ID::BRIDGE_BODY_01, L"Resources\\Textures\\bridge\\bridgebody01.bmp");
	GraphicsHelper::InsertTexure(BRIDGE_TEXTURE_ID::BRIDGE_BODY_02, L"Resources\\Textures\\bridge\\bridgebody02.bmp");

	GraphicsHelper::InsertTexure(BRIDGE_TEXTURE_ID::BRIDGE_HEAD_00, L"Resources\\Textures\\bridge\\bridgehead00.bmp");
	GraphicsHelper::InsertTexure(BRIDGE_TEXTURE_ID::BRIDGE_HEAD_01, L"Resources\\Textures\\bridge\\bridgehead01.bmp");
	GraphicsHelper::InsertTexure(BRIDGE_TEXTURE_ID::BRIDGE_HEAD_02, L"Resources\\Textures\\bridge\\bridgehead02.bmp");

	GraphicsHelper::InsertTexure(BRIDGE_TEXTURE_ID::BRIDGE_TAIL_00, L"Resources\\Textures\\bridge\\bridgetail00.bmp");
	GraphicsHelper::InsertTexure(BRIDGE_TEXTURE_ID::BRIDGE_TAIL_01, L"Resources\\Textures\\bridge\\bridgetail01.bmp");
	GraphicsHelper::InsertTexure(BRIDGE_TEXTURE_ID::BRIDGE_TAIL_02, L"Resources\\Textures\\bridge\\bridgetail02.bmp");

	GraphicsHelper::InsertTexure(BRIDGE_TEXTURE_ID::BRIDGE_EXPLOSION_00, L"Resources\\Textures\\bridge\\bridgeexplosion00.bmp");
	GraphicsHelper::InsertTexure(BRIDGE_TEXTURE_ID::BRIDGE_EXPLOSION_01, L"Resources\\Textures\\bridge\\bridgeexplosion01.bmp");
	GraphicsHelper::InsertTexure(BRIDGE_TEXTURE_ID::BRIDGE_EXPLOSION_02, L"Resources\\Textures\\bridge\\bridgeexplosion02.bmp");
	GraphicsHelper::InsertTexure(BRIDGE_TEXTURE_ID::BRIDGE_EXPLOSION_03, L"Resources\\Textures\\bridge\\bridgeexplosion03.bmp");
	GraphicsHelper::InsertTexure(BRIDGE_TEXTURE_ID::BRIDGE_EXPLOSION_04, L"Resources\\Textures\\bridge\\bridgeexplosion04.bmp");
	GraphicsHelper::InsertTexure(BRIDGE_TEXTURE_ID::BRIDGE_EXPLOSION_05, L"Resources\\Textures\\bridge\\bridgeexplosion05.bmp");
	GraphicsHelper::InsertTexure(BRIDGE_TEXTURE_ID::BRIDGE_EXPLOSION_06, L"Resources\\Textures\\bridge\\bridgeexplosion06.bmp");
	GraphicsHelper::InsertTexure(BRIDGE_TEXTURE_ID::BRIDGE_EXPLOSION_07, L"Resources\\Textures\\bridge\\bridgeexplosion07.bmp");
	GraphicsHelper::InsertTexure(BRIDGE_TEXTURE_ID::BRIDGE_EXPLOSION_08, L"Resources\\Textures\\bridge\\bridgeexplosion08.bmp");
	GraphicsHelper::InsertTexure(BRIDGE_TEXTURE_ID::BRIDGE_EXPLOSION_09, L"Resources\\Textures\\bridge\\bridgeexplosion09.bmp");
	GraphicsHelper::InsertTexure(BRIDGE_TEXTURE_ID::BRIDGE_EXPLOSION_10, L"Resources\\Textures\\bridge\\bridgeexplosion10.bmp");
	GraphicsHelper::InsertTexure(BRIDGE_TEXTURE_ID::BRIDGE_EXPLOSION_11, L"Resources\\Textures\\bridge\\bridgeexplosion11.bmp");
	GraphicsHelper::InsertTexure(BRIDGE_TEXTURE_ID::BRIDGE_EXPLOSION_12, L"Resources\\Textures\\bridge\\bridgeexplosion12.bmp");
	GraphicsHelper::InsertTexure(BRIDGE_TEXTURE_ID::BRIDGE_EXPLOSION_13, L"Resources\\Textures\\bridge\\bridgeexplosion13.bmp");
	GraphicsHelper::InsertTexure(BRIDGE_TEXTURE_ID::BRIDGE_EXPLOSION_14, L"Resources\\Textures\\bridge\\bridgeexplosion14.bmp");

	OutputDebugString(L"Bridge Texture Loaded Successfully\n");
}

void Bridge::LoadAnimations()
{
	if (HasAnimations::hasBeenLoaded.value) {
		return;
	}
	HasAnimations::hasBeenLoaded.value = true;

	GraphicsHelper::InsertAnimation(BRIDGE_ANIMATION_ID::BODY, 165,
		{
			{BRIDGE_SPRITE_ID::BRIDGE_BODY_00, 0},
			{BRIDGE_SPRITE_ID::BRIDGE_BODY_01, 0},
			{BRIDGE_SPRITE_ID::BRIDGE_BODY_02, 0},
		});

	GraphicsHelper::InsertAnimation(BRIDGE_ANIMATION_ID::HEAD, 165,
		{
			{BRIDGE_SPRITE_ID::BRIDGE_HEAD_00, 0},
			{BRIDGE_SPRITE_ID::BRIDGE_HEAD_01, 0},
			{BRIDGE_SPRITE_ID::BRIDGE_HEAD_02, 0},
		});

	GraphicsHelper::InsertAnimation(BRIDGE_ANIMATION_ID::TAIL, 165,
		{
			{BRIDGE_SPRITE_ID::BRIDGE_TAIL_00, 0},
			{BRIDGE_SPRITE_ID::BRIDGE_TAIL_01, 0},
			{BRIDGE_SPRITE_ID::BRIDGE_TAIL_02, 0},
		});

	GraphicsHelper::InsertAnimation(BRIDGE_ANIMATION_ID::EXPLOSION, 50,
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
