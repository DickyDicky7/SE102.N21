#pragma once

#include "Common.h"
#include "GraphicsHelper.h"
#include "GraphicsDatabase.h"

template <class T>
class HasAnimations
{

public:

	HasAnimations();
	virtual ~HasAnimations();
	virtual void LoadAnimations() = 0;
	virtual void SetAnimation(ANIMATION_ID, D3DXVECTOR3, DIRECTION);

protected:

	INT currentFrame;
	ULONGLONG lastFrameTime;
	static Bool<T> hasBeenLoaded;

};

template <class T>
inline HasAnimations<T>::HasAnimations()
{
	currentFrame = -1;
	lastFrameTime = -1;
}

template <class T>
inline HasAnimations<T>::~HasAnimations()
{
}

template <class T>
inline void HasAnimations<T>::SetAnimation(ANIMATION_ID animationId, D3DXVECTOR3 position, DIRECTION direction)
{
	ULONGLONG now = GetTickCount64();

	if (currentFrame == -1 || std::cmp_greater_equal(currentFrame, GraphicsDatabase::animations[animationId].second.size()))
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
			if (std::cmp_greater_equal(currentFrame, GraphicsDatabase::animations[animationId].second.size())) currentFrame = 0;
		}
	}

	GraphicsHelper::DrawSprite(GraphicsDatabase::sprites[GraphicsDatabase::animations[animationId].second[currentFrame].first], position, direction);
}

template <class T>
Bool<T> HasAnimations<T>::hasBeenLoaded
{
	false
};