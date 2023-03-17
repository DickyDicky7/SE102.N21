#pragma once

#include "Common.h"
#include "GraphicsHelper.h"
#include "GraphicsDatabase.h"

class GraphicsState
{

public:

	GraphicsState();
	virtual ~GraphicsState();
	virtual void SetAnimation(ANIMATION_ID, D3DXVECTOR3);

protected:

	INT currentFrame;
	ULONGLONG lastFrameTime;

};

