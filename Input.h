#pragma once

#include "Common.h"

class Input
{

public:

	Input(HINSTANCE, HWND);
	virtual ~Input();

	virtual void Capture();
	virtual INT IsKey(INT);
	virtual INT IsButton(INT);
	virtual void SetMouseEventListener(void (*mouseEventListener)(Input&));
	virtual void SetKeyboardEventListener(void (*keyboardEventListener)(Input&));

protected:

	char keys[256];
	DIMOUSESTATE buttons;
	LPDIRECTINPUT8 input;
	LPDIRECTINPUTDEVICE8 mouse;
	LPDIRECTINPUTDEVICE8 keyboard;
	std::vector<void (*)(Input&)> mouseEventListeners;
	std::vector<void (*)(Input&)> keyboardEventListeners;

};

