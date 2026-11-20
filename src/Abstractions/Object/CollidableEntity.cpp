#include <algorithm>
#include "CollidableEntity.h"
#include "Constants.h"

CollidableEntity::CollidableEntity()
	: _self(nullptr), _surfaceEntity(nullptr), _isAbSurface(false), _isBeSurface(false), _isNeToSurfaceLe(false), _isNeToSurfaceRi(false)
{
}

CollidableEntity::~CollidableEntity()
{
}

bool CollidableEntity::AABBCheck(Entity* targetEntity)
{
	if (!targetEntity || !this->_self)
	{
		return false;
	}

	return targetEntity->GetL() <= this->_self->GetR()
		&& targetEntity->GetR() >= this->_self->GetL()
		&& targetEntity->GetT() >= this->_self->GetB()
		&& targetEntity->GetB() <= this->_self->GetT();
}

void CollidableEntity::CollideWith(Entity* targetEntity)
{
	if (!targetEntity || !this->_self)
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

AABBSweepResult CollidableEntity::AABBSweep(Entity* targetEntity)
{
	if (!targetEntity || !this->_self)
	{
		return AABBSweepResult();
	}

	AABBSweepResult aabbSweepResultX = this->AABBSweepX(targetEntity);
	AABBSweepResult aabbSweepResultY = this->AABBSweepY(targetEntity);

	if (aabbSweepResultX.isCollided && aabbSweepResultY.isCollided)
	{
		float entryTime = (std::max)(aabbSweepResultX.enTime, aabbSweepResultY.enTime);
		float exitTime = (std::min)(aabbSweepResultX.exTime, aabbSweepResultY.exTime);

		if (exitTime < 0.0f || entryTime > exitTime || entryTime > 1.0f)
		{
			return AABBSweepResult();
		}

		if (aabbSweepResultX.enTime >= aabbSweepResultY.enTime)
		{
			return aabbSweepResultX;
		}
		else
		{
			return aabbSweepResultY;
		}
	}
	if (aabbSweepResultX.isCollided)
	{
		if (this->_self->GetVY() == 0.0f && this->_self->GetB() <= targetEntity->GetT() && this->_self->GetT() >= targetEntity->GetB())
		{
			return aabbSweepResultX;
		}
	}
	if (aabbSweepResultY.isCollided)
	{
		if (this->_self->GetVX() == 0.0f && this->_self->GetL() <= targetEntity->GetR() && this->_self->GetR() >= targetEntity->GetL())
		{
			return aabbSweepResultY;
		}
	}

	return AABBSweepResult();
}

AABBSweepResult CollidableEntity::AABBSweepX(Entity* targetEntity)
{
	AABBSweepResult aabbSweepResult{};

	if (!targetEntity || !this->_self || this->_self->GetVX() == 0.0f)
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
	= (this->_self->GetVX() > 0.0f ? this->_self->GetR() : this->_self->GetL()) + this->_self->GetVX() * aabbSweepResult.enTime;
	aabbSweepResult.contactY
	= (this->_self->GetB() + this->_self->GetT()) / 2.0f;
	aabbSweepResult.isCollided = true;
	aabbSweepResult.surfaceEntity
	=  targetEntity;

	return aabbSweepResult;
}

AABBSweepResult CollidableEntity::AABBSweepY(Entity* targetEntity)
{
	AABBSweepResult aabbSweepResult{};

	if (!targetEntity || !this->_self || this->_self->GetVY() == 0.0f)
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
	= (this->_self->GetVY() > 0.0f ? this->_self->GetT() : this->_self->GetB()) + this->_self->GetVY() * aabbSweepResult.enTime;
	aabbSweepResult.isCollided = true;
	aabbSweepResult.surfaceEntity
	=  targetEntity;

	return aabbSweepResult;
}

