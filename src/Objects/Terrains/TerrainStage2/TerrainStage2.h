#pragma once


#include "Common.h"
#include "Entity.h"
#include "Terrain.h"
#include "HasSprites.h"
#include "HasTextures.h"
#include "HasAnimations.h"


class TerrainStage2 : public Terrain<TerrainStage2>, public Entity, public HasTextures<TerrainStage2>, public HasSprites<TerrainStage2>, public HasAnimations<TerrainStage2>
{

public:

	TerrainStage2();
	virtual ~TerrainStage2();

	void Update() override;
	void Render() override;
	void HandleInput(Input&) override;

	void LoadSprites() override;
	void LoadTextures() override;
	void LoadAnimations() override;
	void SetAnimationId(const std::string&) override;

};

