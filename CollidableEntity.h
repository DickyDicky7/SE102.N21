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

	BOOL isOnSurface;
	BOOL isUnderSurface;
	BOOL isNextToSurface;

};

inline BOOL CollidableEntity::AABBCheck(Entity* targetEntity)
{
	auto other = (Entity*)targetEntity;

	FLOAT selfB = self->GetY();
	FLOAT selfT = self->GetY() + self->GetH();
	FLOAT selfL = self->GetX() - self->GetW() / 2.0f;
	FLOAT selfR = self->GetX() + self->GetW() / 2.0f;

	FLOAT otherB = other->GetY();
	FLOAT otherT = other->GetY() + other->GetH();
	FLOAT otherL = other->GetX() - other->GetW() / 2.0f;
	FLOAT otherR = other->GetX() + other->GetW() / 2.0f;

	return
		otherL <= selfR &&
		otherR >= selfL &&
		otherT >= selfB &&
		otherB <= selfT
		;
}

inline void CollidableEntity::CollideWith(Entity* targetEntity)
{
	AABBSweepResult aabbSweepResult = AABBSweep(targetEntity);
	if (aabbSweepResult.isCollided)
	{
		surfaceEntity = NULL;
		surfaceEntity = targetEntity;

		DynamicResolveOnCollision(aabbSweepResult);
		auto collidingEntity = dynamic_cast<CollidableEntity*>(targetEntity);
		if (collidingEntity) collidingEntity->StaticResolveOnCollision(aabbSweepResult);
	}
	else
	{
		DynamicResolveNoCollision(               );
		auto collidingEntity = dynamic_cast<CollidableEntity*>(targetEntity);
		if (collidingEntity) collidingEntity->StaticResolveNoCollision(               );
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

	auto  other = targetEntity;
	FLOAT selfL = self->GetX() - self->GetW() / 2.0f;
	FLOAT selfR = self->GetX() + self->GetW() / 2.0f;
	FLOAT otherL = other->GetX() - other->GetW() / 2.0f;
	FLOAT otherR = other->GetX() + other->GetW() / 2.0f;

	FLOAT enTimeX = 0.0f;
	FLOAT exTimeX = 0.0f;
	if (self->GetVX() > 0.0f)
	{
		enTimeX = (otherL - selfR) / self->GetVX();
		exTimeX = (otherR - selfL) / self->GetVX();
	}
	if (self->GetVX() < 0.0f)
	{
		enTimeX = (otherR - selfL) / self->GetVX();
		exTimeX = (otherL - selfR) / self->GetVX();
	}
	aabbSweepResult.enTime = enTimeX;
	aabbSweepResult.exTime = exTimeX;

	if (aabbSweepResult.enTime > aabbSweepResult.exTime
	||  aabbSweepResult.enTime > 1.0f
	||  aabbSweepResult.exTime < 0.0f)
	{
		return aabbSweepResult;
	}

	aabbSweepResult.normalX    =  self->GetVX() > 0.0f ? -1.0f : 1.0f;
	aabbSweepResult.normalY    =  0.0f;
	aabbSweepResult.contactX   = (self->GetVX() > 0.0f ? selfR : selfL) + aabbSweepResult.normalX * self->GetVX() * aabbSweepResult.enTime;
	aabbSweepResult.contactY   =  self->GetY() + self->GetH() / 2.0f;
	aabbSweepResult.isCollided =  1;

	return aabbSweepResult;
}

inline AABBSweepResult CollidableEntity::AABBSweepY(Entity* targetEntity)
{
	AABBSweepResult aabbSweepResult{};

	if (self->GetVY() == 0.0f)
	{
		return aabbSweepResult;
	}

	auto  other = targetEntity;
	FLOAT selfB = self->GetY();
	FLOAT selfT = self->GetY() + self->GetH();
	FLOAT otherB = other->GetY();
	FLOAT otherT = other->GetY() + other->GetH();

	FLOAT enTimeY = 0.0f;
	FLOAT exTimeY = 0.0f;
	if (self->GetVY() < 0.0f)
	{
		enTimeY = (otherT - selfB) / self->GetVY();
		exTimeY = (otherB - selfT) / self->GetVY();
	}
	if (self->GetVY() > 0.0f)
	{
		enTimeY = (otherB - selfT) / self->GetVY();
		exTimeY = (otherT - selfB) / self->GetVY();
	}
	aabbSweepResult.enTime = enTimeY;
	aabbSweepResult.exTime = exTimeY;

	if (aabbSweepResult.enTime > aabbSweepResult.exTime
	||  aabbSweepResult.enTime > 1.0f
	||  aabbSweepResult.exTime < 0.0f)
	{
		return aabbSweepResult;
	}

	aabbSweepResult.normalX    =  0.0f;
	aabbSweepResult.normalY    =  self->GetVY() > 0.0f ? -1.0f : 1.0f;
	aabbSweepResult.contactX   =  self->GetX ();
	aabbSweepResult.contactY   = (self->GetVY() > 0.0f ? selfT : selfB) + aabbSweepResult.normalY * self->GetVY() * aabbSweepResult.enTime;
	aabbSweepResult.isCollided =  1;

	return aabbSweepResult;
}

