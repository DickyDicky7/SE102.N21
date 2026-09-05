#include "Input.h"
#include <algorithm>

// DIRECTINPUT_VERSION has to be a macro (see Common.h); this keeps the constant
// that mirrors it honest.
static_assert(DIRECTINPUT_VERSION == Constants::Input::DIRECT_INPUT_VERSION,
              "DIRECTINPUT_VERSION and Constants::Input::DIRECT_INPUT_VERSION disagree");

Input::Input(HINSTANCE hInstance, HWND hWnd)
	: _keys{}, _mouseST{}, _input(nullptr), _mouse(nullptr), _keyboard(nullptr)
{
	// Each step used the pointer the previous one was supposed to fill without
	// ever asking whether it had been.  DirectInput8Create failing (or either
	// CreateDevice failing - a machine with no mouse is enough) therefore meant
	// a dereference of an uninitialised pointer here in the constructor.
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION,
		IID_IDirectInput8, reinterpret_cast<LPVOID*>(&this->_input), nullptr)) || !this->_input)
	{
		this->_input = nullptr;
		return;
	}

	if (SUCCEEDED(this->_input->CreateDevice(GUID_SysMouse, &this->_mouse, nullptr)) && this->_mouse)
	{
		this->_mouse->SetDataFormat(&c_dfDIMouse);
		this->_mouse->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
		this->_mouse->Acquire();
	}
	else
	{
		this->_mouse = nullptr;
	}

	if (SUCCEEDED(this->_input->CreateDevice(GUID_SysKeyboard, &this->_keyboard, nullptr)) && this->_keyboard)
	{
		this->_keyboard->SetDataFormat(&c_dfDIKeyboard);
		this->_keyboard->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
		this->_keyboard->Acquire();
	}
	else
	{
		this->_keyboard = nullptr;
	}
}

Input::~Input()
{
	if (this->_keyboard) this->_keyboard->Unacquire(), this->_keyboard->Release();
	if (this->_mouse) this->_mouse->Unacquire(), this->_mouse->Release();
	if (this->_input) this->_input->Release();
}

void Input::Capture()
{
	// A failed read must not leave the PREVIOUS frame's state in place: held keys
	// would stay held, and _mouseST.lZ is a relative axis, so a stale non-zero
	// wheel delta re-triggers a camera zoom step every frame until it clears.
	// Hence the zero on any failure, not just on the two focus-loss codes - those
	// are only the ones an Acquire can fix.
	HRESULT result = this->_mouse ? this->_mouse->GetDeviceState(sizeof(this->_mouseST), static_cast<LPVOID>(&this->_mouseST))
	                              : E_FAIL;
	if (FAILED(result))
	{
		// Only a focus-loss code is worth an Acquire; anything else falls straight
		// through to the zero below.
		if ((result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED) && SUCCEEDED(this->_mouse->Acquire()))
			result = this->_mouse->GetDeviceState(sizeof(this->_mouseST), static_cast<LPVOID>(&this->_mouseST));

		if (FAILED(result))
			this->_mouseST = {};
	}

	result = this->_keyboard ? this->_keyboard->GetDeviceState(sizeof(this->_keys), static_cast<LPVOID>(&this->_keys))
	                         : E_FAIL;
	if (FAILED(result))
	{
		if ((result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED) && SUCCEEDED(this->_keyboard->Acquire()))
			result = this->_keyboard->GetDeviceState(sizeof(this->_keys), static_cast<LPVOID>(&this->_keys));

		if (FAILED(result))
			std::ranges::fill(this->_keys, 0);
	}
}

bool Input::IsKey(int keyCode)
{
	return (this->_keys[keyCode] & 0x80) != 0;
}

bool Input::IsButton(int buttonCode)
{
	return (this->_mouseST.rgbButtons[buttonCode] & 0x80) != 0;
}

bool Input::IsWheelUp()
{
	return this->_mouseST.lZ > 0;
}

bool Input::IsWheelDown()
{
	return this->_mouseST.lZ < 0;
}

bool Input::IsWheel(LONG wheelDelta)
{
	return this->_mouseST.lZ == wheelDelta;
}




