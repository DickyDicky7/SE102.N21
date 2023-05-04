#pragma once

#include "Input.h"
#include "Common.h"
#include "Entity.h"

template <class T, class E> requires std::derived_from<E, Entity>
class State
{

public:

	State();
	virtual ~State();

	virtual void Exit(E&) = 0;
	virtual void Enter(E&) = 0;
	virtual void Render(E&) = 0;

	virtual T* Update(E&) = 0;
	virtual T* HandleInput(E&, Input&) = 0;

};

template <class T, class E> requires std::derived_from<E, Entity>
inline State<T, E>::State()
{
}

template <class T, class E> requires std::derived_from<E, Entity>
inline State<T, E>::~State()
{
}
