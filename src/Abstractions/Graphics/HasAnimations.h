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

	virtual int GetCurrentFrame() const;
	virtual float GetCurrentFrameW() const;
	virtual float GetCurrentFrameH() const;

	virtual void SetAnimation(const ANIMATION_ID& animationId, D3DXVECTOR3 position, DIRECTION movingDirection, float angle);
	virtual void LoadAnimations()									   = 0;

protected:

	int _currentFrame;
	float _currentFrameW;
	float _currentFrameH;
	ULONGLONG _lastFrameTime;
	static inline bool _hasBeenLoaded = false;

};

template <class T>
inline HasAnimations<T>::HasAnimations()
{
	this->_currentFrame  = -1	 ;
	this->_currentFrameW = +0.0f;
	this->_currentFrameH = +0.0f;
	this->_lastFrameTime = +0	 ;
}

template <class T>
inline HasAnimations<T>::~HasAnimations()
{
}

template <class T>
inline int HasAnimations<T>::GetCurrentFrame() const
{
	return this->_currentFrame;
}

template <class T>
inline float HasAnimations<T>::GetCurrentFrameW() const
{
	return this->_currentFrameW;
}

template <class T>
inline float HasAnimations<T>::GetCurrentFrameH() const
{
	return this->_currentFrameH;
}

template <class T>
inline void HasAnimations<T>::SetAnimation(const ANIMATION_ID& animationId, D3DXVECTOR3 position, DIRECTION movingDirection, float angle)
{
	ULONGLONG now = GetTickCount64();
	auto animIt = GraphicsDatabase::animations.find(animationId);
	if (animIt == GraphicsDatabase::animations.end()) return;

	std::vector<std::tuple<SPRITE_ID, TIME>>& frames = std::get<
		std::vector<std::tuple<SPRITE_ID, TIME>>>(animIt->second);

	// A registered animation with no frames would otherwise reach frames[0]
	// below: the clamp underneath resets _currentFrame to 0 rather than leaving
	// it out of range, so an empty vector is the one size it cannot make safe.
	if (frames.empty()) return;

	if (this->_currentFrame == -1 || std::cmp_greater_equal(this->_currentFrame, frames.size()))
	{
		this->_currentFrame = 0;
		this->_lastFrameTime = now;
	}
	else
	{
		if (now - this->_lastFrameTime > std::get<TIME>(frames[this->_currentFrame]))
		{
			this->_currentFrame++;
			this->_lastFrameTime = now;
			if (std::cmp_greater_equal(this->_currentFrame, frames.size())) this->_currentFrame = 0;
		}
	}

	auto spriteIt = GraphicsDatabase::sprites.find(std::get<SPRITE_ID>(frames[this->_currentFrame]));
	if (spriteIt == GraphicsDatabase::sprites.end()) return;

	SPRITE& currentSprite = spriteIt->second;
	const RECT& rect = std::get<RECT>(currentSprite);
	this->_currentFrameW = static_cast<float>(rect.right  - rect.left);
	this->_currentFrameH = static_cast<float>(rect.bottom - rect.top );

	GraphicsHelper::DrawSprite(currentSprite, position, movingDirection, angle);
}
