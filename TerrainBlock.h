#pragma once


#include "Common.h"
#include "Entity.h"
#include "CollidableEntity.h"


enum class TERRAIN_BLOCK_TYPE { NONE, WALL, WATER, THROUGHABLE, NON_THROUGHABLE, };


class TerrainBlock : public Entity, public CollidableEntity
{

public:

	std::string name;

	TerrainBlock();
	virtual ~TerrainBlock();
	TERRAIN_BLOCK_TYPE type;

	void Update() override;
	void Render() override;
	void HandleInput(Input&) override;

	void  StaticResolveNoCollision(               ) override;
	void  StaticResolveOnCollision(AABBSweepResult) override;
	void DynamicResolveNoCollision(               ) override;
	void DynamicResolveOnCollision(AABBSweepResult) override;

};

