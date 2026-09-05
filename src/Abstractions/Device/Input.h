#pragma once

#include "Common.h"

class Input
{

public:

	Input(HINSTANCE hInstance, HWND hWnd);
	virtual ~Input();

	virtual void Capture();

	virtual bool IsKey(int keyCode);
	virtual bool IsButton(int buttonCode);

	virtual bool IsWheelUp();
	virtual bool IsWheelDown();
	virtual bool IsWheel(LONG wheelDelta);

	//virtual void SetMouseEventListener(MOUSE_EVENT_HANDLER);
	//virtual void SetKeyboardEventListener(KEYBOARD_EVENT_HANDLER);

protected:

	char _keys[Constants::Input::KEYBOARD_KEY_COUNT];
	DIMOUSESTATE _mouseST;
	LPDIRECTINPUT8 _input;
	LPDIRECTINPUTDEVICE8 _mouse;
	LPDIRECTINPUTDEVICE8 _keyboard;
	//std::vector<MOUSE_EVENT_HANDLER> mouseEventListeners;
	//std::vector<KEYBOARD_EVENT_HANDLER> keyboardEventListeners;

};

