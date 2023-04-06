#pragma once

#include "Common.h"

template <class T, class O>
class Enemy
{

public:

	Enemy();
	virtual ~Enemy();

	virtual T* SetHP(INT = 0);
	virtual T* SetTarget(const O* = NULL);

	virtual INT GetHP() const;
	virtual const O* GetTarget() const;

protected:

	T* self = NULL;

	INT hp;
	const O* target;

};

template <class T, class O>
inline Enemy<T, O>::Enemy() : hp(0), target(NULL)
{
}

template <class T, class O>
inline Enemy<T, O>::~Enemy()
{
}

template <class T, class O>
inline T* Enemy<T, O>::SetHP(INT hp) { this->hp = hp; return self; }

template <class T, class O>
inline T* Enemy<T, O>::SetTarget(const O* target) { this->target = target; return self; }

template <class T, class O>
inline INT Enemy<T, O>::GetHP() const { return hp; }

template <class T, class O>
inline const O* Enemy<T, O>::GetTarget() const { return target; }