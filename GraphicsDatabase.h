#pragma once

#include "Common.h"

class GraphicsDatabase
{

public:

	static std::unordered_map<SPRITE_ID, SPRITE> sprites;
	static std::unordered_map<TEXTURE_ID, TEXTURE> textures;
	static std::unordered_map<ANIMATION_ID, ANIMATION> animations;

};