#pragma once

#include "Common.h"
#include "Entity.h"

struct AABBSweepResult
{
	BOOL isCollided = 0;

	FLOAT enTime   = -std::numeric_limits<FLOAT>::infinity();
	FLOAT exTime   = +std::numeric_limits<FLOAT>::infinity();
	FLOAT normalX  = +std::numeric_limits<FLOAT>::infinity();
	FLOAT normalY  = +std::numeric_limits<FLOAT>::infinity();
	FLOAT contactX = +std::numeric_limits<FLOAT>::infinity();
	FLOAT contactY = +std::numeric_limits<FLOAT>::infinity();

	Entity* surfaceEntity = NULL;
};

class CollidableEntity
{

public:

	CollidableEntity();
	virtual ~CollidableEntity();
	virtual void  StaticResolveNoCollision(               ) = 0;
	virtual void  StaticResolveOnCollision(AABBSweepResult) = 0;
	virtual void DynamicResolveNoCollision(               ) = 0;
	virtual void DynamicResolveOnCollision(AABBSweepResult) = 0;

	BOOL AABBCheck(Entity*);

	void CollideWith(Entity*);

	AABBSweepResult AABBSweep(Entity*);

	AABBSweepResult AABBSweepX(Entity*);

	AABBSweepResult AABBSweepY(Entity*);

protected:

	Entity* self;
	Entity* surfaceEntity;

	BOOL isAbSurface;
	BOOL isBeSurface;
	BOOL isNeToSurfaceLe;
	BOOL isNeToSurfaceRi;

};

inline BOOL CollidableEntity::AABBCheck(Entity* targetEntity)
{
	return targetEntity->GetL() <= self->GetR()
		&& targetEntity->GetR() >= self->GetL()
		&& targetEntity->GetT() >= self->GetB()
		&& targetEntity->GetB() <= self->GetT()
		;
}

inline void CollidableEntity::CollideWith(Entity* targetEntity)
{
	AABBSweepResult aabbSweepResult = AABBSweep(targetEntity);
	aabbSweepResult.enTime -= 0.1f;
	if (aabbSweepResult.isCollided)
	{
		DynamicResolveOnCollision(aabbSweepResult);
		auto collidingEntity = dynamic_cast<CollidableEntity*>(targetEntity);
		if  (collidingEntity) collidingEntity->StaticResolveOnCollision(aabbSweepResult);
	}
	else
	{
		DynamicResolveNoCollision(               );
		auto collidingEntity = dynamic_cast<CollidableEntity*>(targetEntity);
		if  (collidingEntity) collidingEntity->StaticResolveNoCollision(               );
	}
}

inline AABBSweepResult CollidableEntity::AABBSweep(Entity* targetEntity)
{
	AABBSweepResult aabbSweepResultX = AABBSweepX(targetEntity);
	AABBSweepResult aabbSweepResultY = AABBSweepY(targetEntity);

	if (aabbSweepResultX.isCollided && aabbSweepResultY.isCollided)
	{
		if (aabbSweepResultX.enTime > aabbSweepResultY.enTime)
		{
			return aabbSweepResultX;
		}
		if (aabbSweepResultX.enTime < aabbSweepResultY.enTime)
		{
			return aabbSweepResultY;
		}
	}
	if (aabbSweepResultX.isCollided)
	{
		if (AABBCheck(targetEntity))
			return aabbSweepResultX;
	}
	if (aabbSweepResultY.isCollided)
	{
		if (AABBCheck(targetEntity))
			return aabbSweepResultY;
	}

	return AABBSweepResult();
}

inline AABBSweepResult CollidableEntity::AABBSweepX(Entity* targetEntity)
{
	AABBSweepResult aabbSweepResult{};

	if (self->GetVX() == 0.0f)
	{
		return aabbSweepResult;
	}

	FLOAT enTimeX = 0.0f;
	FLOAT exTimeX = 0.0f;
	if (self->GetVX() > 0.0f)
	{
		enTimeX = (targetEntity->GetL() - self->GetR()) / self->GetVX();
		exTimeX = (targetEntity->GetR() - self->GetL()) / self->GetVX();
	}
	if (self->GetVX() < 0.0f)
	{
		enTimeX = (targetEntity->GetR() - self->GetL()) / self->GetVX();
		exTimeX = (targetEntity->GetL() - self->GetR()) / self->GetVX();
	}
	aabbSweepResult.enTime = enTimeX;
	aabbSweepResult.exTime = exTimeX;

	if (aabbSweepResult.enTime > aabbSweepResult.exTime
	||  aabbSweepResult.enTime > 1.0f
	||  aabbSweepResult.exTime < 0.0f)
	{
		return aabbSweepResult;
	}

	aabbSweepResult.normalX
	=  self->GetVX() > 0.0f ? -1.0f : 1.0f;
	aabbSweepResult.normalY
	=  0.0f;
	aabbSweepResult.contactX
	= (self->GetVX() > 0.0f ? self->GetR() : self->GetL()) + aabbSweepResult.normalX * self->GetVX() * aabbSweepResult.enTime;
	aabbSweepResult.contactY
	=  self->GetT () / 2.0f;
	aabbSweepResult.isCollided
	=  1;
	aabbSweepResult.surfaceEntity
	=  targetEntity;

	return aabbSweepResult;
}

inline AABBSweepResult CollidableEntity::AABBSweepY(Entity* targetEntity)
{
	AABBSweepResult aabbSweepResult{};

	if (self->GetVY() == 0.0f)
	{
		return aabbSweepResult;
	}

	FLOAT enTimeY = 0.0f;
	FLOAT exTimeY = 0.0f;
	if (self->GetVY() < 0.0f)
	{
		enTimeY = (targetEntity->GetT() - self->GetB()) / self->GetVY();
		exTimeY = (targetEntity->GetB() - self->GetT()) / self->GetVY();
	}
	if (self->GetVY() > 0.0f)
	{
		enTimeY = (targetEntity->GetB() - self->GetT()) / self->GetVY();
		exTimeY = (targetEntity->GetT() - self->GetB()) / self->GetVY();
	}
	aabbSweepResult.enTime = enTimeY;
	aabbSweepResult.exTime = exTimeY;

	if (aabbSweepResult.enTime > aabbSweepResult.exTime
	||  aabbSweepResult.enTime > 1.0f
	||  aabbSweepResult.exTime < 0.0f)
	{
		return aabbSweepResult;
	}

	aabbSweepResult.normalX
	=  0.0f;
	aabbSweepResult.normalY
	=  self->GetVY() > 0.0f ? -1.0f : 1.0f;
	aabbSweepResult.contactX
	=  self->GetX ();
	aabbSweepResult.contactY
	= (self->GetVY() > 0.0f ? self->GetT() : self->GetB()) + aabbSweepResult.normalY * self->GetVY() * aabbSweepResult.enTime;
	aabbSweepResult.isCollided
	=  1;
	aabbSweepResult.surfaceEntity
	=  targetEntity;

	return aabbSweepResult;
}


