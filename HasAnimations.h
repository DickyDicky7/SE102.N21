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

	virtual INT GetCurrentFrame() const;
	virtual FLOAT GetCurrentFrameW() const;
	virtual FLOAT GetCurrentFrameH() const;

	virtual void SetAnimation(ANIMATION_ID, D3DXVECTOR3, DIRECTION, FLOAT);
	virtual void LoadAnimations()									   = 0;

protected:

	INT currentFrame;
	FLOAT currentFrameW;
	FLOAT currentFrameH;
	ULONGLONG lastFrameTime;
	static Bool<T> hasBeenLoaded;

};

template <class T>
inline HasAnimations<T>::HasAnimations()
{
	currentFrame  = -1	 ;
	currentFrameW = +0.0f;
	currentFrameH = +0.0f;
	lastFrameTime = -1	 ;
}

template <class T>
inline HasAnimations<T>::~HasAnimations()
{
}

template <class T>
inline INT HasAnimations<T>::GetCurrentFrame() const
{
	return currentFrame;
}

template<class T>
inline FLOAT HasAnimations<T>::GetCurrentFrameW() const
{
	return currentFrameW;
}

template<class T>
inline FLOAT HasAnimations<T>::GetCurrentFrameH() const
{
	return currentFrameH;
}

template <class T>
inline void HasAnimations<T>::SetAnimation(ANIMATION_ID animationId, D3DXVECTOR3 position, DIRECTION movingDirection, FLOAT angle)
{
	ULONGLONG now = GetTickCount64();
	std::vector<std::tuple<SPRITE_ID, TIME>>& frames = std::get<
	std::vector<std::tuple<SPRITE_ID, TIME>>>(GraphicsDatabase::animations[animationId]);

	if (currentFrame == -1 || std::cmp_greater_equal(currentFrame, frames.size()))
	{
		currentFrame = 0;
		lastFrameTime = now;
	}
	else
	{
		if (now - lastFrameTime > std::get<TIME>(frames[currentFrame]))
		{
			currentFrame++;
			lastFrameTime = now;
			if (std::cmp_greater_equal(currentFrame, frames.size())) currentFrame = 0;
		}
	}

	SPRITE& currentSprite = GraphicsDatabase::sprites[std::get<SPRITE_ID>(frames[currentFrame])];
	RECT* rect = std::get<RECT*>(currentSprite);
	if (rect)
	{
		currentFrameW = (FLOAT)(rect->right  - rect->left);
		currentFrameH = (FLOAT)(rect->bottom - rect->top );
	}

	GraphicsHelper::DrawSprite(currentSprite, position, movingDirection, angle);
}

template <class T>
Bool<T> HasAnimations<T>::hasBeenLoaded
{
	false
};
