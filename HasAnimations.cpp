#include "HasAnimations.h"

HasAnimations::HasAnimations()
{
	currentFrame = -1;
	lastFrameTime = -1;
}

HasAnimations::~HasAnimations()
{
}

void HasAnimations::SetAnimation(ANIMATION_ID animationId, D3DXVECTOR3 position, DIRECTION direction)
{
	ULONGLONG now = GetTickCount64();

	if (currentFrame == -1 || currentFrame >= GraphicsDatabase::animations[animationId].second.size())
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
			if (currentFrame >= GraphicsDatabase::animations[animationId].second.size()) currentFrame = 0;
		}
	}

	GraphicsHelper::DrawSprite(GraphicsDatabase::sprites[GraphicsDatabase::animations[animationId].second[currentFrame].first], position, direction);
}
