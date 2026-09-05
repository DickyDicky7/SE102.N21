#pragma once

#include "Common.h"

template <class T>
class Terrain
{

public:

	Terrain();
	virtual ~Terrain();

	static       void   SetTileset(void* tileset);
	static const void*  GetTileset(             );

	virtual		  void		   SetAnimationId(const std::string& animationId);
	virtual const std::string& GetAnimationId(                              );

protected:

	static void* _tileset;
	std::string _animationId;

};

template <class T>
Terrain<T>:: Terrain()
{
}

template <class T>
Terrain<T>::~Terrain()
{
}

template <class T>
inline       void   Terrain<T>::SetTileset(void* tileset)
{
	Terrain<T>::_tileset = tileset;
}

template <class T>
inline const void*  Terrain<T>::GetTileset(             )
{
		 return _tileset;
}

template <class T>
inline       void         Terrain<T>::SetAnimationId(const std::string& animationId)
{
	 this->_animationId = animationId;
}

template <class T>
inline const std::string& Terrain<T>::GetAnimationId(                              )
{
	return this->_animationId;
}

template <class T>
void* Terrain<T>::_tileset = nullptr;
