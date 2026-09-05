#pragma once

#include "Common.h"
#include "CollidableEntity.h"

// Base template for collidable state representations
template <class T, class E> requires std::derived_from<E, Entity>
class CollidableState
{

public:

	CollidableState();
	virtual ~CollidableState();
	virtual T*  StaticResolveNoCollision(E& entity) = 0;
	virtual T*  StaticResolveOnCollision(E& entity, const AABBSweepResult& aabbSweepResult) = 0;
	virtual T* DynamicResolveNoCollision(E& entity) = 0;
	virtual T* DynamicResolveOnCollision(E& entity, const AABBSweepResult& aabbSweepResult) = 0;

};

template <class T, class E> requires std::derived_from<E, Entity>
inline CollidableState<T, E>::CollidableState()
{
}

template <class T, class E> requires std::derived_from<E, Entity>
inline CollidableState<T, E>::~CollidableState()
{
}
