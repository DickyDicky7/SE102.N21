#pragma once

#include "Common.h"
#include "Entity.h"

template <class T> requires std::derived_from<T, Entity<T>>
class IsCollidable
{

	//IsCollidable instance is a static object (assumedly, although it could be a dynamic object)
	//IsCollidable instance's colliding objects are dynamic objects (based on the view of the instance - the code is assumed that the instance is a static object)
	//IsCollidable instance's colliding objects will also be checked if they are truly dynamic objects
	//If IsCollidable instance is also a dynamic object, we will use "Relative Velocity" to resolve the problem

public:

	IsCollidable();
	virtual ~IsCollidable();

	virtual BOOL IsCollidedWithRay(const D3DXVECTOR2&, const D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&, FLOAT&);

protected:

	Entity<T>* self;
	std::vector<IsCollidable*> contacts;

};

template <class T> requires std::derived_from<T, Entity<T>>
inline IsCollidable<T>::IsCollidable()
{
	contacts.reserve(4);
}

template <class T> requires std::derived_from<T, Entity<T>>
inline IsCollidable<T>::~IsCollidable()
{

}

template <class T> requires std::derived_from<T, Entity<T>>
inline BOOL IsCollidable<T>::IsCollidedWithRay(const D3DXVECTOR2& rayOrigin, const D3DXVECTOR2& rayDirection, D3DXVECTOR2& contactPoint, D3DXVECTOR2& contactNormal, FLOAT& nearHitTime)
{
	contactPoint  = { 0.0f, 0.0f };
	contactNormal = { 0.0f, 0.0f };

	// Length of one division
	D3DXVECTOR2 invDirection
	(
		1.0f / (rayDirection.x == 0.0f ? INFINITY : rayDirection.x), 
		1.0f / (rayDirection.y == 0.0f ? INFINITY : rayDirection.y)
	);
	
	// Calculate intersections with rectangle bounding axes
	D3DXVECTOR2 nearTime = (D3DXVECTOR2(self->GetPosition())										   - rayOrigin) * invDirection;
	D3DXVECTOR2  farTime = (D3DXVECTOR2(self->GetPosition()) + D3DXVECTOR2(self->GetW(), self->GetH()) - rayOrigin) * invDirection;

	if (std::isnan(nearTime.x) || std::isnan(nearTime.y)) return 0;
	if (std::isnan( farTime.x) || std::isnan( farTime.y)) return 0;

	// Sort distances
	if (nearTime.x > farTime.x) std::swap(nearTime.x, farTime.x);
	if (nearTime.y > farTime.y) std::swap(nearTime.y, farTime.y);

	// Early rejection		
	if (nearTime.x > farTime.y || nearTime.y > farTime.x) return 0;

	// Closest 'time' will be the first contact
	nearHitTime = max(nearTime.x, nearTime.y);

	// Furthest 'time' is contact on opposite side of target
	FLOAT farHitTime = min(farTime.x, farTime.y);

	// Reject if ray direction is pointing away from object
	if (farHitTime < 0.0f) return 0;

	// Contact point of collision from parametric line equation
	contactPoint = rayOrigin + nearHitTime * rayDirection;

	if (nearTime.x > nearTime.y)
		if (invDirection.x < 0)
			contactNormal = { +1, 0 };
		else
			contactNormal = { -1, 0 };
	else
	if (nearTime.x < nearTime.y)
		if (invDirection.y < 0)
			contactNormal = { 0, +1 };
		else
			contactNormal = { 0, -1 };

	// Note if nearTime == farTime, collision is principly in a diagonal
	// so pointless to resolve. By returning a contactNormal={0,0} even though its
	// considered a hit, the resolver wont change anything.
	return 1;
}

