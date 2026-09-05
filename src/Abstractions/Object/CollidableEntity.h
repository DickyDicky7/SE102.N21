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

inline bool CollidableEntity::AABBCheck(Entity* targetEntity)
{
	return targetEntity->GetL() <= this->_self->GetR()
		&& targetEntity->GetR() >= this->_self->GetL()
		&& targetEntity->GetT() >= this->_self->GetB()
		&& targetEntity->GetB() <= this->_self->GetT()
		;
}

inline void CollidableEntity::CollideWith(Entity* targetEntity)
{
	if (!targetEntity)
		return;

	AABBSweepResult aabbSweepResult = this->AABBSweep(targetEntity);
	aabbSweepResult.enTime -= Constants::Physics::SWEEP_ENTRY_TIME_BACKOFF;
	if (aabbSweepResult.isCollided)
	{
		this->DynamicResolveOnCollision(aabbSweepResult);
		targetEntity->StaticResolveOnCollision(aabbSweepResult);
	}
	else
	{
		this->DynamicResolveNoCollision();
		targetEntity->StaticResolveNoCollision();
	}
}

inline AABBSweepResult CollidableEntity::AABBSweep(Entity* targetEntity)
{
	AABBSweepResult aabbSweepResultX = this->AABBSweepX(targetEntity);
	AABBSweepResult aabbSweepResultY = this->AABBSweepY(targetEntity);

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
		if (this->AABBCheck(targetEntity))
			return aabbSweepResultX;
	}
	if (aabbSweepResultY.isCollided)
	{
		if (this->AABBCheck(targetEntity))
			return aabbSweepResultY;
	}

	return AABBSweepResult();
}

inline AABBSweepResult CollidableEntity::AABBSweepX(Entity* targetEntity)
{
	AABBSweepResult aabbSweepResult{};

	if (this->_self->GetVX() == 0.0f)
	{
		return aabbSweepResult;
	}

	float enTimeX = 0.0f;
	float exTimeX = 0.0f;
	if (this->_self->GetVX() > 0.0f)
	{
		enTimeX = (targetEntity->GetL() - this->_self->GetR()) / this->_self->GetVX();
		exTimeX = (targetEntity->GetR() - this->_self->GetL()) / this->_self->GetVX();
	}
	if (this->_self->GetVX() < 0.0f)
	{
		enTimeX = (targetEntity->GetR() - this->_self->GetL()) / this->_self->GetVX();
		exTimeX = (targetEntity->GetL() - this->_self->GetR()) / this->_self->GetVX();
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
	=  this->_self->GetVX() > 0.0f ? -1.0f : 1.0f;
	aabbSweepResult.normalY
	=  0.0f;
	aabbSweepResult.contactX
	= (this->_self->GetVX() > 0.0f ? this->_self->GetR() : this->_self->GetL()) + aabbSweepResult.normalX * this->_self->GetVX() * aabbSweepResult.enTime;
	aabbSweepResult.contactY
	=  this->_self->GetT () / 2.0f;
	aabbSweepResult.isCollided = true;
	aabbSweepResult.surfaceEntity
	=  targetEntity;

	return aabbSweepResult;
}

inline AABBSweepResult CollidableEntity::AABBSweepY(Entity* targetEntity)
{
	AABBSweepResult aabbSweepResult{};

	if (this->_self->GetVY() == 0.0f)
	{
		return aabbSweepResult;
	}

	float enTimeY = 0.0f;
	float exTimeY = 0.0f;
	if (this->_self->GetVY() < 0.0f)
	{
		enTimeY = (targetEntity->GetT() - this->_self->GetB()) / this->_self->GetVY();
		exTimeY = (targetEntity->GetB() - this->_self->GetT()) / this->_self->GetVY();
	}
	if (this->_self->GetVY() > 0.0f)
	{
		enTimeY = (targetEntity->GetB() - this->_self->GetT()) / this->_self->GetVY();
		exTimeY = (targetEntity->GetT() - this->_self->GetB()) / this->_self->GetVY();
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
	=  this->_self->GetVY() > 0.0f ? -1.0f : 1.0f;
	aabbSweepResult.contactX
	=  this->_self->GetX ();
	aabbSweepResult.contactY
	= (this->_self->GetVY() > 0.0f ? this->_self->GetT() : this->_self->GetB()) + aabbSweepResult.normalY * this->_self->GetVY() * aabbSweepResult.enTime;
	aabbSweepResult.isCollided = true;
	aabbSweepResult.surfaceEntity
	=  targetEntity;

	return aabbSweepResult;
}


