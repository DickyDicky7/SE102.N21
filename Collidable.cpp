#include "Collidable.h"

Collidable::Collidable()
{
}

Collidable::~Collidable()
{
}

BOOL Collidable::IsCollidedWith(BoundingBox movingBoundingBox, BoundingBox staticBoundingBox)
{
	return movingBoundingBox.left <= staticBoundingBox.right && movingBoundingBox.right >= staticBoundingBox.left && movingBoundingBox.top <= staticBoundingBox.bottom && movingBoundingBox.bottom >= staticBoundingBox.top;
}
