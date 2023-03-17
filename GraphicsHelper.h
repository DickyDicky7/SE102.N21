#pragma once

#include "Common.h"
#include "GraphicsDatabase.h"

class GraphicsHelper
{

public:

	static LPDIRECT3DDEVICE9 device;
	static LPD3DXSPRITE spriteHandler;

	static LPDIRECT3DTEXTURE9 CreateTexture(LPCWSTR);
	static std::pair<RECT*, TEXTURE_ID> CreateSprite(INT, INT, INT, INT, TEXTURE_ID);
	static std::pair<DWORD, std::vector<std::pair<SPRITE_ID, DWORD>>> CreateAnimation(DWORD, std::vector<std::pair<SPRITE_ID, DWORD>>);

	static void DrawSprite(std::pair<RECT*, TEXTURE_ID>, D3DXVECTOR3);

};

