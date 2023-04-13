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
	virtual void LoadAnimations()   = 0;
	virtual void SetAnimation(ANIMATION_ID, D3DXVECTOR3, DIRECTION, FLOAT);

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
inline INT HasAnimations<T>::GetCurrentFrame() const
{
	return currentFrame;
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

	GraphicsHelper::DrawSprite(GraphicsDatabase::sprites[std::get<SPRITE_ID>(frames[currentFrame])], position, movingDirection, angle);
}

template <class T>
Bool<T> HasAnimations<T>::hasBeenLoaded
{
	false
};
