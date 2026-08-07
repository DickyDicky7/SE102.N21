#pragma once

#include "Common.h"

template <class T>
class HasSprites
{

public:

	HasSprites();
	virtual ~HasSprites();
	virtual void LoadSprites() = 0;

protected:

	static Bool<T> hasBeenLoaded;

};

template <class T>
inline HasSprites<T>::HasSprites()
{
}

template <class T>
inline HasSprites<T>::~HasSprites()
{
}

template <class T>
Bool<T> HasSprites<T>::hasBeenLoaded
{
	false
};