#include "CollidableEntity.h"

CollidableEntity::CollidableEntity() : self(NULL), surfaceEntity(NULL)
{
	isAbSurface = 0;
	isBeSurface = 0;
	isNeToSurfaceLe = 0;
	isNeToSurfaceRi = 0;
}

CollidableEntity::~CollidableEntity()
{
}

