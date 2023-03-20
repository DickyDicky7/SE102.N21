#include "Input.h"

Input::Input()
{
	/*DirectInput8Create
	(
		GetModuleHandle(NULL), DIRECTINPUT_VERSION,
		IID_IDirectInput8, (void**)input, NULL
	);
	input->CreateDevice
	(
		GUID_SysKeyboard, &keyboard, NULL
	);*/
}

Input::~Input()
{
	/*if (keyboard) keyboard->Release();
	if (input) input->Release();*/
}
