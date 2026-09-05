#pragma once


#include "Common.h"
#include "Entity.h"
#include "CollidableEntity.h"


class TerrainBlock : public Entity, public CollidableEntity
{

public:

	TerrainBlock();
	virtual ~TerrainBlock();

	void SetTerrainType(TERRAIN_BLOCK_TYPE type) { this->_type = type; }
	TERRAIN_BLOCK_TYPE GetTerrainType() const override { return this->_type; }
	void SetEntityName(std::string_view entityName) { this->_name = entityName; }
	std::string GetEntityName() const override { return this->_name; }

	void Update() override;
	void Render() override;
	void HandleInput(Input& input) override;

	CollidableEntity* AsCollidable() override { return this; }
	void  StaticResolveNoCollision(                               ) override;
	void  StaticResolveOnCollision(AABBSweepResult aabbSweepResult) override;
	void DynamicResolveNoCollision(                               ) override;
	void DynamicResolveOnCollision(AABBSweepResult aabbSweepResult) override;

protected:

	std::string _name;
	TERRAIN_BLOCK_TYPE _type;

};

