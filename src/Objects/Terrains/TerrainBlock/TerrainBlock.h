#pragma once


#include "Common.h"
#include "Entity.h"
#include "CollidableEntity.h"


class TerrainBlock : public Entity, public CollidableEntity
{

public:

	TerrainBlock();
	virtual ~TerrainBlock();

	void SetTerrainType(TERRAIN_BLOCK_TYPE type);
	TERRAIN_BLOCK_TYPE GetTerrainType() const override;
	void SetEntityName(std::string_view entityName);
	std::string GetEntityName() const override;

	void Update() override;
	void Render() override;
	void HandleInput(Input& input) override;

	CollidableEntity* AsCollidable() override;
	void  StaticResolveNoCollision(                               ) override;
	void  StaticResolveOnCollision(AABBSweepResult aabbSweepResult) override;
	void DynamicResolveNoCollision(                               ) override;
	void DynamicResolveOnCollision(AABBSweepResult aabbSweepResult) override;

protected:

	std::string _name;
	TERRAIN_BLOCK_TYPE _type;

};

