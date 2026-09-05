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

	static inline bool _hasBeenLoaded = false;

};

template <class T>
inline HasSprites<T>::HasSprites()
{
}

template <class T>
inline HasSprites<T>::~HasSprites()
{
}
