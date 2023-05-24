#pragma once


#include "Common.h"
#include "Entity.h"
#include "Terrain.h"
#include "HasSprites.h"
#include "HasTextures.h"
#include "HasAnimations.h"


class TerrainStage1 : public Terrain<TerrainStage1>, public Entity, public HasTextures<TerrainStage1>, public HasSprites<TerrainStage1>, public HasAnimations<TerrainStage1>
{

public:

	TerrainStage1();
	virtual ~TerrainStage1();

	void Update() override;
	void Render() override;
	void HandleInput(Input&) override;

	void LoadSprites() override;
	void LoadTextures() override;
	void LoadAnimations() override;
	void SetAnimationId(const std::string&) override;

};

