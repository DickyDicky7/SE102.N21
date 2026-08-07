#pragma once

#include "Common.h"
#include "HasSprites.h"
#include "HasTextures.h"
#include "HasAnimations.h"

class TestingEntity : public HasTextures<TestingEntity>, public HasSprites<TestingEntity>, public HasAnimations<TestingEntity>
{

public:

	void LoadSprites() override;
	void LoadTextures() override;
	void LoadAnimations() override;

};

