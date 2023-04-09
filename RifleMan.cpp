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
	if (HasTextures<RifleMan>::hasBeenLoaded.value) {
		OutputDebugString(L"\n Has been load texture RifleMan \n");
		return;
	}
	HasTextures<RifleMan>::hasBeenLoaded.value = true;

	GraphicsHelper::InsertTexure(RIFLE_MAN_TEXTURE_ID::RIFLE_MAN, L"Resources\\Textures\\rifleman.bmp");
	OutputDebugString(L"\n Load texture RifleMan success \n");

	return;
}
