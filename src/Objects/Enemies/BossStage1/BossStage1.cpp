#include "BossStage1.h"
#include "GraphicsHelper.h"

BossStage1::BossStage1()
{

}

BossStage1::~BossStage1()
{

}

void BossStage1::LoadTextures()
{
	if (HasTextures<BossStage1>::_hasBeenLoaded)
	{
		return;
	}

	HasTextures<BossStage1>::_hasBeenLoaded = true;

	GraphicsHelper::InsertTexture(BOSS_STAGE_1_TEXTURE_ID::BOSS_STAGE_1, L"Resources\\Textures\\boss_stage1.bmp");
	return;
}
