#pragma once

#include "Common.h"

template <class T>
class Terrain
{

public:

	Terrain();
	virtual ~Terrain();

	static       void   SetTileset(void*);
	static const void*& GetTileset(     );

	virtual		  void		   SetAnimationId(const std::string&);
	virtual const std::string& GetAnimationId(                  );

protected:

	static void* tileset;
	std::string  animationId;

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
	Terrain<T>::tileset = tileset;
}

template <class T>
inline const void*& Terrain<T>::GetTileset(             )
{
		 return tileset;
}

template <class T>
inline       void         Terrain<T>::SetAnimationId(const std::string& animationId)
{
	 this->animationId = animationId;
}

template <class T>
inline const std::string& Terrain<T>::GetAnimationId(                              )
{
	return animationId;
}

template <class T>
void* Terrain<T>::tileset = NULL;
