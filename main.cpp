#include <d3d9.h>
#include <time.h>
#include <d3dx9.h>
#include "BillRunning.h"
#include "WindowClass.h"
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")

#define WINDOWCLASS_WINDOWPROC WindowProc
#define WINDOWCLASS_SIZE sizeof(WNDCLASSEX)
#define WINDOWCLASS_CLASSNAME L"WindowClass"
#define WINDOWCLASS_STYLE CS_HREDRAW | CS_VREDRAW 
#define WINDOWCLASS_BACKGROUND (HBRUSH)COLOR_WINDOW
#define WINDOWCLASS_CURSOR LoadCursor(NULL, IDC_ARROW)

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 500
#define WINDOW_PARENT NULL
#define WINDOW_MENUBAR NULL
#define WINDOW_X_POSITION 0
#define WINDOW_Y_POSITION 0
#define WINDOW_EXTRA_STYLE NULL
#define WINDOW_TITLE L"MainWindow"
#define WINDOW_STYLE WS_EX_TOPMOST | WS_POPUP | WS_VISIBLE

LPDIRECT3D9 d3d = NULL;
LPDIRECT3DDEVICE9 d3ddev = NULL;
LPDIRECT3DSURFACE9 backBuffer = NULL;

BaseSprites* billRunning = new BillRunning();

void GameInit(HWND);
void GameRun(HWND);
void GameEnd(HWND);
LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);


int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{

	WindowClass* wc = new WindowClass();
	wc->Clean();
	wc->SetHInstance(hInstance)
		->SetStyle(WINDOWCLASS_STYLE)
		->SetCbSize(WINDOWCLASS_SIZE)
		->SetHCursor(WINDOWCLASS_CURSOR)
		->SetLpfnWndProc(WINDOWCLASS_WINDOWPROC)
		->SetLpszClassName(WINDOWCLASS_CLASSNAME)
		->SetHbrBackground(WINDOWCLASS_BACKGROUND)
		;
	wc->Build();


	HWND hWnd = CreateWindowEx
	(
		WINDOW_EXTRA_STYLE,
		WINDOWCLASS_CLASSNAME,
		WINDOW_TITLE,
		WINDOW_STYLE,
		WINDOW_X_POSITION,
		WINDOW_Y_POSITION,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		WINDOW_PARENT,
		WINDOW_MENUBAR,
		hInstance,
		NULL // used with multiple windows, NULL
	);

	ShowWindow(hWnd, nCmdShow);

	GameInit(hWnd);

	MSG msg;
	BOOL isDone = FALSE;

	while (!isDone)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				OutputDebugString(L"\n\nWM_QUIT called\n\n");
				isDone = TRUE;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			GameRun(hWnd);
		}
	}

	GameEnd(hWnd);

	return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{

	case WM_DESTROY:
		OutputDebugString(L"\n\nWM_DESTROY called\n\n");
		PostQuitMessage(0);
		return 0;
		break;

	case WM_CLOSE:
		OutputDebugString(L"\n\nWM_CLOSE called\n\n");
		PostQuitMessage(0);
		return 0;
		break;

	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

void GameInit(HWND hWnd)
{
	d3d = Direct3DCreate9(D3D_SDK_VERSION);

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.Windowed = TRUE;
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.BackBufferWidth = WINDOW_WIDTH;
	d3dpp.BackBufferHeight = WINDOW_HEIGHT;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;

	d3d->CreateDevice
	(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&d3ddev
	);

	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 0);

	d3ddev->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backBuffer);

	srand(time(NULL));

	billRunning
		->SetX(0)
		->SetY(0)
		->SetW(38)
		->SetH(68)
		->SetDx(3)
		->SetDy(3)
		->SetCurFrame(0)
		->SetAnimDelay(3)
		->SetAnimCount(0)
		->BuildSurfaces(d3ddev);
}

void GameRun(HWND hWnd)
{
	Sleep(60);

	RECT rect{};
	rect.top = billRunning->GetY();
	rect.bottom = billRunning->GetY() + billRunning->GetH();
	rect.left = billRunning->GetX();
	rect.right = billRunning->GetX() + billRunning->GetW();

	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 0);

	d3ddev->BeginScene();

	d3ddev->StretchRect(billRunning->GetSurface(), NULL, backBuffer, &rect, D3DTEXF_NONE);

	d3ddev->EndScene();

	billRunning->SetX(billRunning->GetX() + billRunning->GetDx());
	if (billRunning->GetX() >= WINDOW_WIDTH)
		billRunning->SetX(0);

	d3ddev->Present(NULL, NULL, NULL, NULL);
}

void GameEnd(HWND hWnd)
{
	billRunning->Release();
	backBuffer->Release();
	d3ddev->Release();
	d3d->Release();
}