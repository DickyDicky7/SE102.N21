#pragma once

#include "Common.h"
#include "Entity.h"

struct AABBSweepResult
{
	FLOAT enTime     = -std::numeric_limits<FLOAT>::infinity();
	FLOAT exTime     = +std::numeric_limits<FLOAT>::infinity();
	FLOAT normalX    = 0.0f									  ;
	FLOAT normalY    = 0.0f									  ;
	FLOAT contactX   = +std::numeric_limits<FLOAT>::infinity();
	FLOAT contactY   = +std::numeric_limits<FLOAT>::infinity();
	BOOL  isCollided = 0									  ;
};

struct BoundingBox
{
	FLOAT top   ;
	FLOAT left  ;
	FLOAT right ;
	FLOAT bottom;
};

class Collidable
{

public:

	Collidable();
	virtual ~Collidable();
	// NOTE: REMEMBER TO RESOLVE THE POSITION FIRST
	virtual void ResolveCollision(               ) = 0;
	virtual void ResolveCollision(AABBSweepResult) = 0;

	template <class T> requires std::derived_from<T, Entity<T>>
	AABBSweepResult AABBSweep(T*);

	template <class T> requires std::derived_from<T, Entity<T>>
	AABBSweepResult AABBSweepX(T*);

	template <class T> requires std::derived_from<T, Entity<T>>
	AABBSweepResult AABBSweepY(T*);

	// PURPOSE: FOR RAW CHECKING -- SHOULD NOT USE IT IN REAL GAME
	template <class T> requires std::derived_from<T, Entity<T>>
	BOOL IsCollidedWith(T*);
	BOOL IsCollidedWith(BoundingBox, BoundingBox);

protected:

	Entity<std::any>* self;
	//std::vector<Collidable*> collidingEntities;

};

template <class T> requires std::derived_from<T, Entity<T>>
inline AABBSweepResult Collidable::AABBSweep(T* targetEntity)
{
	AABBSweepResult aabbSweepResultX = AABBSweepX(targetEntity);
	AABBSweepResult aabbSweepResultY = AABBSweepY(targetEntity);

	if (aabbSweepResultX.isCollided && aabbSweepResultY.isCollided)
	{
		if (aabbSweepResultX.enTime > aabbSweepResultY.enTime)
		{
			return aabbSweepResultX;
		}
		else
		if (aabbSweepResultX.enTime < aabbSweepResultY.enTime)
		{
			return aabbSweepResultY;
		}
	}

	return AABBSweepResult();

	//BoundingBox movingBoundingBox{ selfT , selfL , selfR , selfB  };
	//BoundingBox staticBoundingBox{ otherT, otherL, otherR, otherB };
}

template <class T> requires std::derived_from<T, Entity<T>>
inline AABBSweepResult Collidable::AABBSweepX(T* targetEntity)
{
	auto other = (Entity<std::any>*)targetEntity;

	FLOAT selfL = self->GetX() - self->GetW() / 2.0f;
	FLOAT selfR = self->GetX() + self->GetW() / 2.0f;

	FLOAT otherL = other->GetX() - other->GetW() / 2.0f;
	FLOAT otherR = other->GetX() + other->GetW() / 2.0f;

	AABBSweepResult aabbSweepResult{};

	FLOAT enTimeX;
	FLOAT exTimeX;

	if (self->GetVX() > 0.0f)
	{
		enTimeX = (otherL - selfR) / self->GetVX();
		exTimeX = (otherR - selfL) / self->GetVX();
	}
	else
	if (self->GetVX() < 0.0f)
	{
		enTimeX = (otherR - selfL) / self->GetVX();
		exTimeX = (otherL - selfR) / self->GetVX();
	}
	else
	{
		enTimeX = -std::numeric_limits<FLOAT>::infinity();
		exTimeX = +std::numeric_limits<FLOAT>::infinity();
	}

	aabbSweepResult.enTime = enTimeX;
	aabbSweepResult.exTime = exTimeX;

	if (aabbSweepResult.enTime > aabbSweepResult.exTime
	||  aabbSweepResult.enTime > 1.0f
	||  aabbSweepResult.exTime < 0.0f
	||  self->GetVX() == 0.0f)
	{
		return aabbSweepResult;
	}

	aabbSweepResult.isCollided = 1;

	aabbSweepResult.normalY = 0.0f;
	aabbSweepResult.normalX = self->GetVX() > 0.0f ? -1.0f : 1.0f;

	aabbSweepResult.contactY = (self->GetY () + self->GetH() / 2.0f);
	aabbSweepResult.contactX = (self->GetVX() > 0.0f ? selfR : selfL) + aabbSweepResult.normalX * self->GetVX() * aabbSweepResult.enTime;

	return aabbSweepResult;
}

template <class T> requires std::derived_from<T, Entity<T>>
inline AABBSweepResult Collidable::AABBSweepY(T* targetEntity)
{
	auto other = (Entity<std::any>*)targetEntity;

	FLOAT selfB = self->GetY();
	FLOAT selfT = self->GetY() + self->GetH();

	FLOAT otherB = other->GetY();
	FLOAT otherT = other->GetY() + other->GetH();

	AABBSweepResult aabbSweepResult{};

	FLOAT enTimeY;
	FLOAT exTimeY;

	if (self->GetVY() < 0.0f)
	{
		enTimeY = (otherT - selfB) / self->GetVY();
		exTimeY = (otherB - selfT) / self->GetVY();
	}
	else
	if (self->GetVY() > 0.0f)
	{
		enTimeY = (otherB - selfT) / self->GetVY();
		exTimeY = (otherT - selfB) / self->GetVY();
	}
	else
	{
		enTimeY = -std::numeric_limits<FLOAT>::infinity();
		exTimeY = +std::numeric_limits<FLOAT>::infinity();
	}

	aabbSweepResult.enTime = enTimeY;
	aabbSweepResult.exTime = exTimeY;

	if (aabbSweepResult.enTime > aabbSweepResult.exTime 
	||  aabbSweepResult.enTime > 1.0f
	||  aabbSweepResult.exTime < 0.0f
	||  self->GetVY() == 0.0f)
	{
		return aabbSweepResult;
	}

	aabbSweepResult.isCollided = 1;

	aabbSweepResult.normalX = 0.0f;
	aabbSweepResult.normalY = self->GetVY() > 0.0f ? -1.0f : 1.0f;
	
	aabbSweepResult.contactX = (self->GetX ());
	aabbSweepResult.contactY = (self->GetVY() > 0.0f ? selfT : selfB) + aabbSweepResult.normalY * self->GetVY() * aabbSweepResult.enTime;

	return aabbSweepResult;
}

template <class T> requires std::derived_from<T, Entity<T>>
inline BOOL Collidable::IsCollidedWith(T* targetEntity)
{
	auto other = (Entity<std::any>*)targetEntity;

	FLOAT t =  other->GetY()						 - (self->GetY() - self->GetH())	   ;
	FLOAT l = (other->GetX() - other->GetW() / 2.0f) - (self->GetX() + self->GetW() / 2.0f);
	FLOAT r = (other->GetX() + other->GetW() / 2.0f) - (self->GetX() - self->GetW() / 2.0f);
	FLOAT b = (other->GetY() - other->GetH()	   ) -  self->GetY()					   ;

	return !(t < 0 || l > 0 || r < 0 || b > 0);
}
