#include "CollidableEntity.h"

CollidableEntity::CollidableEntity()
{
	self = NULL;
	surfaceEntity = NULL;

	isOnSurface = 0;
	isUnderSurface = 0;
	isNextToSurface = 0;
}

CollidableEntity::~CollidableEntity()
{
}
