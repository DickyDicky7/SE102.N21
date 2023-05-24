#pragma once

#include "Common.h"

template <class T>
class Enemy
{

public:

	Enemy();
	virtual ~Enemy();

	virtual void SetHP(INT = 0);
	virtual void SetTarget(const T* = NULL);

	virtual INT GetHP() const;
	virtual const T* GetTarget() const;

protected:

	INT hp;
	const T* target;

};

template <class T>
inline Enemy<T>::Enemy() : hp(0), target(NULL)
{
}

template <class T>
inline Enemy<T>::~Enemy()
{
}

template <class T>
inline void Enemy<T>::SetHP(INT hp) { this->hp = hp; }

template <class T>
inline void Enemy<T>::SetTarget(const T* target) { this->target = target; }

template <class T>
inline INT Enemy<T>::GetHP() const { return hp; }

template <class T>
inline const T* Enemy<T>::GetTarget() const { return target; }