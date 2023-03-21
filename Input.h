#pragma once

#include "Common.h"

class Input
{

public:

	Input(HINSTANCE, HWND);
	virtual ~Input();

	virtual INT Is(INT);
	virtual void Capture();

protected:

	char keys[256];
	LPDIRECTINPUT8 input;
	LPDIRECTINPUTDEVICE8 keyboard;

};

