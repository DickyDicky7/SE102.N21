#pragma once

#include "Common.h"
#include "Entity.h"

class CollidableEntity
{

public:

	CollidableEntity();
	virtual ~CollidableEntity();
	virtual void  StaticResolveNoCollision(                               ) = 0;
	virtual void  StaticResolveOnCollision(AABBSweepResult aabbSweepResult) = 0;
	virtual void DynamicResolveNoCollision(                               ) = 0;
	virtual void DynamicResolveOnCollision(AABBSweepResult aabbSweepResult) = 0;

	bool AABBCheck(Entity* targetEntity);

	void CollideWith(Entity* targetEntity);

	AABBSweepResult AABBSweep(Entity* targetEntity);

	AABBSweepResult AABBSweepX(Entity* targetEntity);

	AABBSweepResult AABBSweepY(Entity* targetEntity);

protected:

	Entity* _self;
	Entity* _surfaceEntity;

	bool _isAbSurface;
	bool _isBeSurface;
	bool _isNeToSurfaceLe;
	bool _isNeToSurfaceRi;

};


