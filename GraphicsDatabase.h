#pragma once

#include "Common.h"

class GraphicsDatabase
{

public:

	static std::unordered_map<TEXTURE_ID, LPDIRECT3DTEXTURE9> textures;
	static std::unordered_map<SPRITE_ID, std::pair<RECT*, TEXTURE_ID>> sprites;
	static std::unordered_map<ANIMATION_ID, std::pair<DWORD, std::vector<std::pair<SPRITE_ID, DWORD>>>> animations;

};