#pragma once

#include "Common.h"
#include "GraphicsDatabase.h"

class GraphicsHelper
{

public:

	static LPDIRECT3DDEVICE9   device;
	static LPD3DXSPRITE spriteHandler;

	static TEXTURE   CreateTexture(LPCWSTR);
	static SPRITE    CreateSprite(INT, INT, INT, INT, DIRECTION, TEXTURE_ID);
	static ANIMATION CreateAnimation(DEFAULT_TIME, std::vector<std::tuple<SPRITE_ID, TIME>>);

	static void DrawSprite(SPRITE, D3DXVECTOR3, DIRECTION, FLOAT);

	static void InsertTexure(TEXTURE_ID, LPCWSTR);
	static void InsertSprite(SPRITE_ID, INT, INT, INT, INT, DIRECTION, TEXTURE_ID);
	static void InsertAnimation(ANIMATION_ID, DEFAULT_TIME, std::vector<std::tuple<SPRITE_ID, TIME>>);

};