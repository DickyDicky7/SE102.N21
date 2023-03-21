#pragma once

#include "Common.h"
#include "GraphicsHelper.h"
#include "GraphicsDatabase.h"

class HasAnimations
{

public:

	HasAnimations();
	virtual ~HasAnimations();
	virtual void SetAnimation(ANIMATION_ID, D3DXVECTOR3, DIRECTION);

protected:

	INT currentFrame;
	ULONGLONG lastFrameTime;

};

