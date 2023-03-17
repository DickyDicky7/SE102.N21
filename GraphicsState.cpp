#include "GraphicsState.h"

GraphicsState::GraphicsState()
{
	currentFrame = -1;
	lastFrameTime = -1;
}

GraphicsState::~GraphicsState()
{

}

void GraphicsState::SetAnimation(ANIMATION_ID animationId, D3DXVECTOR3 position)
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
}