#include "CollidableEntity.h"

CollidableEntity::CollidableEntity()
	: _self(nullptr), _surfaceEntity(nullptr), _isAbSurface(false), _isBeSurface(false), _isNeToSurfaceLe(false), _isNeToSurfaceRi(false)
{
}

CollidableEntity::~CollidableEntity()
{
}

