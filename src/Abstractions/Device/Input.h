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

	virtual BOOL IsWheelUp();
	virtual BOOL IsWheelDown();
	virtual BOOL IsWheel(LONG);

	//virtual void SetMouseEventListener(MOUSE_EVENT_HANDLER);
	//virtual void SetKeyboardEventListener(KEYBOARD_EVENT_HANDLER);

protected:

	char keys[256];
	DIMOUSESTATE mouseST;
	LPDIRECTINPUT8 input;
	LPDIRECTINPUTDEVICE8 mouse;
	LPDIRECTINPUTDEVICE8 keyboard;
	//std::vector<MOUSE_EVENT_HANDLER> mouseEventListeners;
	//std::vector<KEYBOARD_EVENT_HANDLER> keyboardEventListeners;

};

