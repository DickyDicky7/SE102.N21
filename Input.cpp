#include "Input.h"

Input::Input(HINSTANCE hInstance, HWND hWnd)
{

	DirectInput8Create
	(
		hInstance, DIRECTINPUT_VERSION,
		IID_IDirectInput8, (LPVOID*)&input, NULL
	);

	input->CreateDevice
	(
		GUID_SysMouse, &mouse, NULL
	);
	input->CreateDevice
	(
		GUID_SysKeyboard, &keyboard, NULL
	);

	mouse->SetDataFormat(&c_dfDIMouse);
	keyboard->SetDataFormat(&c_dfDIKeyboard);

	mouse->SetCooperativeLevel
	(
		hWnd,
		DISCL_NONEXCLUSIVE | DISCL_FOREGROUND
	);
	keyboard->SetCooperativeLevel
	(
		hWnd,
		DISCL_NONEXCLUSIVE | DISCL_FOREGROUND
	);

	mouse->Acquire();
	keyboard->Acquire();

}

Input::~Input()
{
	if (keyboard) keyboard->Unacquire(), keyboard->Release();
	if (mouse) mouse->Unacquire(), mouse->Release();
	if (input) input->Release();
}

void Input::Capture()
{

	mouse->GetDeviceState
	(
		sizeof(mouseST), (LPVOID)&mouseST
	);
	keyboard->GetDeviceState
	(
		sizeof(keys), (LPVOID)&keys
	);

	//for (auto& mouseEventListener : mouseEventListeners)
	//{
	//	mouseEventListener(mouse, buttons);
	//}
	//for (auto& keyboardEventListener : keyboardEventListeners)
	//{
	//	keyboardEventListener(keyboard, keys);
	//}

	//_RPT1(0, "X: %d ; Y: %d ; Z: %d\n", buttons.lX, buttons.lY, buttons.lZ);

}

INT Input::IsKey(INT keyCode)
{
	return keys[keyCode] & 0x80;
}

INT Input::IsButton(INT buttonCode)
{
	return mouseST.rgbButtons[buttonCode] & 0x80;
}

BOOL Input::IsWheelUp()
{
	return mouseST.lZ > 0;
}

BOOL Input::IsWheelDown()
{
	return mouseST.lZ < 0;
}

BOOL Input::IsWheel(LONG lZ)
{
	return mouseST.lZ == lZ;
}

//void Input::SetMouseEventListener(MOUSE_EVENT_HANDLER mouseEventListener) { mouseEventListeners.push_back(mouseEventListener); }
//
//void Input::SetKeyboardEventListener(KEYBOARD_EVENT_HANDLER keyboardEventListener) { keyboardEventListeners.push_back(keyboardEventListener); }




