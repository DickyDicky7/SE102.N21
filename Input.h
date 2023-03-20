#pragma once

#include "Common.h"

class Input
{

public:

	Input();
	virtual ~Input();

protected:

	LPDIRECTINPUT8 input;
	LPDIRECTINPUTDEVICE8 keyboard;

};

