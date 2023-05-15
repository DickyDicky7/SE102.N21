#pragma once


#include "Common.h"
#include "Entity.h"
#include "CollidableEntity.h"


class TerrainBlock : public Entity, public CollidableEntity
{

public:

	TerrainBlock();
	virtual ~TerrainBlock();
	
	void Update() override;
	void Render() override;
	void HandleInput(Input&) override;

	void  StaticResolveNoCollision(               ) override;
	void  StaticResolveOnCollision(AABBSweepResult) override;
	void DynamicResolveNoCollision(               ) override;
	void DynamicResolveOnCollision(AABBSweepResult) override;

};

