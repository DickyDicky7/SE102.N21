#include "WindowClass.h"


#define WC_STYLE CS_HREDRAW | CS_VREDRAW
#define WC_CBSIZE sizeof(WNDCLASSEX)

#define WC_HCURSOR LoadCursor(NULL, IDC_ARROW)
#define WC_HBRBACKGROUND (HBRUSH)COLOR_WINDOW

#define WC_LPFNWNDPROC WindowProc
#define WC_LPSZCLASSNAME L"WindowClass1"

#define WINDOW_EX_STYLE NULL
#define WINDOW_X_POS 0
#define WINDOW_Y_POS 0
#define WINDOW_TITLE L"Our First Windowed Program"
#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT 500
#define WINDOW_PARENT NULL
#define WINDOW_MENUBAR NULL


LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{


	WindowClass* wc = new WindowClass();
	wc->Clean();

	wc
		->SetStyle(WC_STYLE)
		->SetCbSize(WC_CBSIZE)

		->SetHCursor(WC_HCURSOR)
		->SetHInstance(hInstance)
		->SetHbrBackground(WC_HBRBACKGROUND)

		->SetLpfnWndProc(WC_LPFNWNDPROC)
		->SetLpszClassName(WC_LPSZCLASSNAME);

	wc->Build();


	HWND hWnd = CreateWindowEx
	(
		WINDOW_EX_STYLE,
		WC_LPSZCLASSNAME,
		WINDOW_TITLE,
		WS_OVERLAPPEDWINDOW,
		WINDOW_X_POS,
		WINDOW_Y_POS,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		WINDOW_PARENT,
		WINDOW_MENUBAR,
		hInstance,
		NULL // used with multiple windows, NULL
	);

	ShowWindow(hWnd, nCmdShow);


	MSG msg;

	while (1)
	{
		// Check to see if any messages are waiting in the queue
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// Translate the message and dispatch it to WindowProc()
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// If the message is WM_QUIT, exit the while loop
		if (msg.message == WM_QUIT)
			break;

		// Run game code here
		// ...
		// ...
	}

	// return this part of the WM_QUIT message to Windows
	return msg.wParam;

	return 0;
}

// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// sort through and find what code to run for the message given
	switch (message)
	{
		// this message is read when the window is closed
	case WM_DESTROY:
	{
		// close the application entirely
		PostQuitMessage(0);
		return 0;
	} break;
	}

	// Handle any messages the switch statement didn't
	return DefWindowProc(hWnd, message, wParam, lParam);
}