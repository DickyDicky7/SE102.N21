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

	template <class T> requires std::derived_from<T, Entity<T>>
	BOOL AABBCheck(T*);

	template <class T> requires std::derived_from<T, Entity<T>>
	void CollideWith(T*);

	template <class T> requires std::derived_from<T, Entity<T>>
	AABBSweepResult AABBSweep(T*);

	template <class T> requires std::derived_from<T, Entity<T>>
	AABBSweepResult AABBSweepX(T*);

	template <class T> requires std::derived_from<T, Entity<T>>
	AABBSweepResult AABBSweepY(T*);

protected:

	Entity<std::any>* self;
	Entity<std::any>* surfaceEntity;

	BOOL isAbSurface;
	BOOL isBeSurface;
	BOOL isNeToSurfaceLe;
	BOOL isNeToSurfaceRi;

};

template <class T> requires std::derived_from<T, Entity<T>>
inline BOOL CollidableEntity::AABBCheck(T* targetEntity)
{
	auto other = (Entity<std::any>*)targetEntity;

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

template <class T> requires std::derived_from<T, Entity<T>>
inline void CollidableEntity::CollideWith(T* targetEntity)
{
	AABBSweepResult aabbSweepResult = AABBSweep(targetEntity);
	if (aabbSweepResult.isCollided)
	{
		surfaceEntity = NULL;
		surfaceEntity = (Entity<std::any>*)targetEntity;

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

template <class T> requires std::derived_from<T, Entity<T>>
inline AABBSweepResult CollidableEntity::AABBSweep(T* targetEntity)
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

template <class T> requires std::derived_from<T, Entity<T>>
inline AABBSweepResult CollidableEntity::AABBSweepX(T* targetEntity)
{
	AABBSweepResult aabbSweepResult{};

	if (self->GetVX() == 0.0f)
	{
		return aabbSweepResult;
	}

	auto  other = (Entity<std::any>*)targetEntity;
	FLOAT selfL = self->GetX() - self->GetW() / 2.0f;
	FLOAT selfR = self->GetX() + self->GetW() / 2.0f;
	FLOAT otherL = other->GetX() - other->GetW() / 2.0f;
	FLOAT otherR = other->GetX() + other->GetW() / 2.0f;

	FLOAT enTimeX;
	FLOAT exTimeX;
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

	aabbSweepResult.normalX
	=  self->GetVX() > 0.0f ? -1.0f : 1.0f;
	aabbSweepResult.normalY
	=  0.0f;
	aabbSweepResult.contactX
	= (self->GetVX() > 0.0f ? selfR : selfL) + aabbSweepResult.normalX * self->GetVX() * aabbSweepResult.enTime;
	aabbSweepResult.contactY
	=  self->GetY () + self->GetH() / 2.0f;
	aabbSweepResult.isCollided
	=  1;

	return aabbSweepResult;
}

template <class T> requires std::derived_from<T, Entity<T>>
inline AABBSweepResult CollidableEntity::AABBSweepY(T* targetEntity)
{
	AABBSweepResult aabbSweepResult{};

	if (self->GetVY() == 0.0f)
	{
		return aabbSweepResult;
	}

	auto  other = (Entity<std::any>*)targetEntity;
	FLOAT selfB = self->GetY();
	FLOAT selfT = self->GetY() + self->GetH();
	FLOAT otherB = other->GetY();
	FLOAT otherT = other->GetY() + other->GetH();

	FLOAT enTimeY;
	FLOAT exTimeY;
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

	aabbSweepResult.normalX
	=  0.0f;
	aabbSweepResult.normalY
	=  self->GetVY() > 0.0f ? -1.0f : 1.0f;
	aabbSweepResult.contactX
	=  self->GetX ();
	aabbSweepResult.contactY
	= (self->GetVY() > 0.0f ? selfT : selfB) + aabbSweepResult.normalY * self->GetVY() * aabbSweepResult.enTime;
	aabbSweepResult.isCollided
	=  1;

	return aabbSweepResult;
}

