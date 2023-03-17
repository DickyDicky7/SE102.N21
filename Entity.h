#pragma once

#include "Common.h"
#include "GraphicsHelper.h"
#include "GraphicsDatabase.h"

template <class T>
class Entity
{

public:

	Entity();
	virtual ~Entity();
	virtual void Update() = 0;

	virtual T* SetX(FLOAT = 0.0f);
	virtual T* SetY(FLOAT = 0.0f);
	virtual T* SetVX(FLOAT = 0.0f);
	virtual T* SetVY(FLOAT = 0.0f);
	virtual T* SetAnimation(ANIMATION_ID);

	virtual FLOAT GetX() const;
	virtual FLOAT GetY() const;
	virtual FLOAT GetVX() const;
	virtual FLOAT GetVY() const;

protected:

	T* self = NULL;

	FLOAT vx;
	FLOAT vy;
	INT currentFrame;
	D3DXVECTOR3 position;
	ULONGLONG lastFrameTime;

};

template <class T>
inline Entity<T>::Entity() : position(0.0f, 0.0f, 0.0f), vx(0.0f), vy(0.0f)
{
	currentFrame = -1;
	lastFrameTime = -1;
	OutputDebugString(L"\n\nBaseEntity's constructor called\n\n");
}

template <class T>
inline Entity<T>::~Entity()
{
	/*if (self)
	{
		delete self;
		self = NULL;
	}*/
	OutputDebugString(L"\n\nBaseEntity's destructor called\n\n");
}

template <class T>
inline T* Entity<T>::SetX(FLOAT x) { position.x = x; return self; }

template <class T>
inline T* Entity<T>::SetY(FLOAT y) { position.y = y; return self; }

template <class T>
inline T* Entity<T>::SetVX(FLOAT vx) { this->vx = vx; return self; }

template <class T>
inline T* Entity<T>::SetVY(FLOAT vy) { this->vy = vy; return self; }

template<class T>
inline T* Entity<T>::SetAnimation(ANIMATION_ID animationId)
{
	ULONGLONG now = GetTickCount64();

	if (currentFrame == -1)
	{
		currentFrame = 0;
		lastFrameTime = now;
	}
	else
	{
		if (now - lastFrameTime > GraphicsDatabase::animations[animationId].second[currentFrame].second)
		{
			currentFrame++;
			lastFrameTime = now;
			if (currentFrame == GraphicsDatabase::animations[animationId].second.size()) currentFrame = 0;
		}
	}

	GraphicsHelper::DrawSprite(GraphicsDatabase::sprites[GraphicsDatabase::animations[animationId].second[currentFrame].first], position);

	return self;
}

template <class T>
inline FLOAT Entity<T>::GetX() const { return position.x; }

template <class T>
inline FLOAT Entity<T>::GetY() const { return position.y; }

template <class T>
inline FLOAT Entity<T>::GetVX() const { return vx; }

template <class T>
inline FLOAT Entity<T>::GetVY() const { return vy; }