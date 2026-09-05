#pragma once

#include "Common.h"

template <class T>
class HasTextures
{

public:

	HasTextures();
	virtual ~HasTextures();
	virtual void LoadTextures() = 0;

protected:

	static inline bool _hasBeenLoaded = false;

};

template <class T>
inline HasTextures<T>::HasTextures()
{
}

template <class T>
inline HasTextures<T>::~HasTextures()
{
}
