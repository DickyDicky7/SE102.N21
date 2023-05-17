#pragma once

#include "Common.h"
#include "CollidableEntity.h"

// DERIVING THIS CLASS OR JUST USE dynamic_cast<> AND THEN CHECK IF THE POINTER IS NULL
// dynamic_cast<> COULD BE USED FOR BOTH UPCASTING AND DOWNCASTING
// JUST CAST A POINTER TO A TYPE AND CHECK IF IT IS NULL (NULL MEANS THE POINTER IS NOT AN INSTANCE OF THAT TYPE)
template <class T, class E> requires std::derived_from<E, Entity>
class CollidableState
{

public:

	CollidableState();
	virtual ~CollidableState();
	virtual T*  StaticResolveNoCollision(E&) = 0;
	virtual T*  StaticResolveOnCollision(E&, const AABBSweepResult&) = 0;
	virtual T* DynamicResolveNoCollision(E&) = 0;
	virtual T* DynamicResolveOnCollision(E&, const AABBSweepResult&) = 0;

};

template <class T, class E> requires std::derived_from<E, Entity>
inline CollidableState<T, E>::CollidableState()
{
}

template <class T, class E> requires std::derived_from<E, Entity>
inline CollidableState<T, E>::~CollidableState()
{
}
