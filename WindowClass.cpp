#include "WindowClass.h"

void WindowClass::Build() { RegisterClassEx(&wce); }
void WindowClass::Clean() { ZeroMemory(&wce, sizeof(WNDCLASSEX)); }

WindowClass* WindowClass::SetStyle(UINT style) { wce.style = style; return this; }
WindowClass* WindowClass::SetCbSize(UINT cbSize) { wce.cbSize = cbSize; return this; }
WindowClass* WindowClass::SetCbClsExtra(int cbClsExtra) { wce.cbClsExtra = cbClsExtra; return this; }
WindowClass* WindowClass::SetCbWndExtra(int cbWndExtra) { wce.cbWndExtra = cbWndExtra; return this; }

WindowClass* WindowClass::SetHIcon(HICON hIcon) { wce.hIcon = hIcon; return this; }
WindowClass* WindowClass::SetHIconSm(HICON hIconSm) { wce.hIconSm = hIconSm; return this; }
WindowClass* WindowClass::SetHCursor(HCURSOR hCursor) { wce.hCursor = hCursor; return this; }
WindowClass* WindowClass::SetHInstance(HINSTANCE hInstance) { wce.hInstance = hInstance; return this; }
WindowClass* WindowClass::SetHbrBackground(HBRUSH hbrBackground) { wce.hbrBackground = hbrBackground; return this; }

WindowClass* WindowClass::SetLpfnWndProc(WNDPROC lpfnWndProc) { wce.lpfnWndProc = lpfnWndProc; return this; }
WindowClass* WindowClass::SetLpszMenuName(LPCWSTR lpszMenuName) { wce.lpszMenuName = lpszMenuName; return this; }
WindowClass* WindowClass::SetLpszClassName(LPCWSTR lpszClassName) { wce.lpszClassName = lpszClassName; return this; }
