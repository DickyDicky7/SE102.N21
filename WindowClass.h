#pragma once

#include <Windows.h>

class WindowClass
{

public:

	void Build();
	void Clean();

	WindowClass* SetStyle(UINT);
	WindowClass* SetCbSize(UINT);
	WindowClass* SetCbClsExtra(int);
	WindowClass* SetCbWndExtra(int);

	WindowClass* SetHIcon(HICON);
	WindowClass* SetHIconSm(HICON);
	WindowClass* SetHCursor(HCURSOR);
	WindowClass* SetHInstance(HINSTANCE);
	WindowClass* SetHbrBackground(HBRUSH);

	WindowClass* SetLpfnWndProc(WNDPROC);
	WindowClass* SetLpszMenuName(LPCWSTR);
	WindowClass* SetLpszClassName(LPCWSTR);

private:

	WNDCLASSEX wce;

};