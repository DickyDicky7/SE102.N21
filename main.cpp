#include <d3d9.h> // direct3d - directx 9
#include "WindowClass.h"


// include the Direct3D Library file
#pragma comment (lib, "d3d9.lib")


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


// global declarations
LPDIRECT3D9 d3d;    // the pointer to our Direct3D interface
LPDIRECT3DDEVICE9 d3ddev;    // the pointer to the device class

// function prototypes
void initD3D(HWND hWnd);    // sets up and initializes Direct3D
void render_frame(void);    // renders a single frame
void cleanD3D(void);    // closes Direct3D and releases memory


LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{

	WindowClass* wc = new WindowClass();
	wc->Clean();
	wc->SetStyle(WC_STYLE)->SetCbSize(WC_CBSIZE)->SetLpszClassName(WC_LPSZCLASSNAME)
		->SetHCursor(WC_HCURSOR)->SetHInstance(hInstance)->SetHbrBackground(WC_HBRBACKGROUND)->SetLpfnWndProc(WC_LPFNWNDPROC);
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

	// set up and initialize Direct3D
	initD3D(hWnd);


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
		{
			OutputDebugString(L"\n\nWM_QUIT called\n\n");
			break;
		}

		render_frame();
		// Run game code here
		// ...
		// ...
	}

	// clean up DirectX and COM
	cleanD3D();

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
		OutputDebugString(L"\n\nWM_DESTROY called\n\n");
		// close the application entirely
		PostQuitMessage(0);
		return 0;
	} break;
	case WM_CLOSE:
	{
		OutputDebugString(L"\n\nWM_CLOSE called\n\n");
		// close the application entirely
		PostQuitMessage(0);
		return 0;
	} break;
	}

	// Handle any messages the switch statement didn't
	return DefWindowProc(hWnd, message, wParam, lParam);
}

// this function initializes and prepares Direct3D for use
void initD3D(HWND hWnd)
{
	d3d = Direct3DCreate9(D3D_SDK_VERSION);    // create the Direct3D interface

	D3DPRESENT_PARAMETERS d3dpp;    // create a struct to hold various device information

	ZeroMemory(&d3dpp, sizeof(d3dpp));    // clear out the struct for use
	d3dpp.Windowed = TRUE;    // program windowed, not fullscreen
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;    // discard old frames
	d3dpp.hDeviceWindow = hWnd;    // set the window to be used by Direct3D
	
	// create a device class using this information and information from the d3dpp stuct
	d3d->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&d3ddev);
}
int GSpd = 1;
int BSpd = 1;
int RSpd = 1;
int R = 255;
int G = 255;
int B = 255;
// this is the function used to render a single frame
void render_frame(void)
{
	// clear the window to a deep blue
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(R, G, B), 1.0f, 0);

	d3ddev->BeginScene();    // begins the 3D scene

	// do 3D rendering on the back buffer here

	if (R == 255 || R == 0) RSpd = -RSpd;

	if (G == 255 || G == 0) GSpd = -GSpd;
	G += GSpd;

	if (B == 255 || B == 0) BSpd = -BSpd;
	B += BSpd;

	d3ddev->EndScene();    // ends the 3D scene

	d3ddev->Present(NULL, NULL, NULL, NULL);    // displays the created frame
}

// this is the function that cleans up Direct3D and COM
void cleanD3D(void)
{
	d3ddev->Release();    // close and release the 3D device
	d3d->Release();    // close and release Direct3D
}