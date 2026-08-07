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
	// A failed read must not leave the PREVIOUS frame's state in place: held keys
	// would stay held, and mouseST.lZ is a relative axis, so a stale non-zero
	// wheel delta re-triggers a camera zoom step every frame until it clears.
	// Hence the zero on any failure, not just on the two focus-loss codes - those
	// are only the ones an Acquire can fix.
	HRESULT result = mouse->GetDeviceState(sizeof(mouseST), (LPVOID)&mouseST);
	if (FAILED(result))
	{
		// Only a focus-loss code is worth an Acquire; anything else falls straight
		// through to the zero below.
		if ((result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED) && SUCCEEDED(mouse->Acquire()))
			result = mouse->GetDeviceState(sizeof(mouseST), (LPVOID)&mouseST);

		if (FAILED(result))
			ZeroMemory(&mouseST, sizeof(mouseST));
	}

	result = keyboard->GetDeviceState(sizeof(keys), (LPVOID)&keys);
	if (FAILED(result))
	{
		if ((result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED) && SUCCEEDED(keyboard->Acquire()))
			result = keyboard->GetDeviceState(sizeof(keys), (LPVOID)&keys);

		if (FAILED(result))
			ZeroMemory(keys, sizeof(keys));
	}
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




