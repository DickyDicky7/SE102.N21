#include "RifleMan.h"
#include "HasTextures.h"
#include "GraphicsHelper.h"

RifleMan::RifleMan()
{

}

RifleMan::~RifleMan()
{

}

void RifleMan::LoadTextures()
{
	if (HasTextures<RifleMan>::_hasBeenLoaded) {
		return;
	}
	HasTextures<RifleMan>::_hasBeenLoaded = true;

	GraphicsHelper::InsertTexture(RIFLE_MAN_TEXTURE_ID::RIFLE_MAN, L"Resources\\Textures\\RifleMan.bmp");

	return;
}
