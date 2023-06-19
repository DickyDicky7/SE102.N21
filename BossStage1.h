#pragma once
#include "HasTextures.h"

class BossStage1;

class BossStage1 : public HasTextures<BossStage1>
{
public:
	BossStage1();
	virtual ~BossStage1();

	static void LoadTextures();
};