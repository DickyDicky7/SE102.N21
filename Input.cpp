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
		GUID_SysKeyboard, &keyboard, NULL
	);
	keyboard->SetDataFormat(&c_dfDIKeyboard);
	keyboard->SetCooperativeLevel
	(
		hWnd,
		DISCL_NONEXCLUSIVE | DISCL_FOREGROUND
	);
	keyboard->Acquire();
}

Input::~Input()
{
	if (keyboard) keyboard->Unacquire(), keyboard->Release();
	if (input) input->Release();
}

INT Input::Is(INT keyCode)
{
	return keys[keyCode] & 0x80;
}

void Input::Capture()
{
	keyboard->GetDeviceState(sizeof(keys), (LPVOID)&keys);
}
