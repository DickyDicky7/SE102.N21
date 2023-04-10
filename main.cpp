#include "Input.h"
#include "Camera.h"
#include "Common.h"

#include "Bill.h"
#include "Soldier.h"
#include "WallTurret.h"
#include "BossStage3.h"
#include "ScubaSoldier.h"
#include "TestingEntity.h"

Input* input;
Camera* camera;
LPDIRECT3D9 d3d;
LPDIRECT3DDEVICE9 d3ddev;
LPD3DXSPRITE spriteHandler;

Bill bill;
Soldier soldier;
WallTurret wallTurret;
BossStage3 bossStage3;
ScubaSoldier scubaSoldier;

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void LoadD3D(HWND hWnd);
void CleanD3D(void);
void LoadAssets();

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	HWND hWnd;
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX); wc.style = CS_HREDRAW | CS_VREDRAW; wc.lpfnWndProc = WindowProc; wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;	wc.lpszClassName = L"WindowClass";
	RegisterClassEx(&wc);

	hWnd = CreateWindowEx
	(
		NULL, L"WindowClass", L"Contra", WS_OVERLAPPEDWINDOW, 0, 0,
		SCREEN_WIDTH, SCREEN_HEIGHT, NULL, NULL, hInstance, NULL
	);
	ShowWindow(hWnd, nCmdShow);

	LoadD3D(hWnd);

	input = new Input
	(
		hInstance, hWnd
	);
	camera = new Camera();
	camera->SetSpeedX(3.0f);
	camera->SetLastX(bill.GetX());

	LoadAssets();

	MSG msg;
	while (TRUE)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT)
			break;

		input->Capture();
		bill.HandleInput(*input);
		soldier.HandleInput(*input);
		bossStage3.HandleInput(*input);
		scubaSoldier.HandleInput(*input);

		bill.Update();
		soldier.Update();
		bossStage3.Update();
		scubaSoldier.Update();

		wallTurret.CalculateBillAngle(&bill);
		wallTurret.Update();

		camera->CaptureX(bill.GetX());

		d3ddev->SetTransform(D3DTS_VIEW, &camera->GetViewMatrix());
		d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 0);
		d3ddev->BeginScene();

		bill.Render();
		soldier.Render();
		bossStage3.Render();
		wallTurret.Render();
		scubaSoldier.Render();

		d3ddev->EndScene();
		d3ddev->Present(NULL, NULL, NULL, NULL);
	}

	CleanD3D();
	delete input;
	input = NULL;

	return msg.wParam;
}

void LoadAssets()
{
	bill.LoadTextures();
	bill.LoadSprites();
	bill.LoadAnimations();

	soldier.LoadTextures();
	soldier.LoadSprites();
	soldier.LoadAnimations();
	soldier.SetTarget(&bill);

	bossStage3.LoadTextures();
	bossStage3.LoadSprites();
	bossStage3.LoadAnimations();
	bossStage3.SetTarget(&bill);

	wallTurret.LoadTextures();
	wallTurret.LoadSprites();
	wallTurret.LoadAnimations();
	wallTurret.SetTarget(&bill);

	scubaSoldier.LoadTextures();
	scubaSoldier.LoadSprites();
	scubaSoldier.LoadAnimations();
	scubaSoldier.SetTarget(&bill);

	TestingEntity testingEntity;
	testingEntity.LoadTextures();
	testingEntity.LoadSprites();
	testingEntity.LoadAnimations();
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
		break;

	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
		break;

	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}


void LoadD3D(HWND hWnd)
{
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3d = Direct3DCreate9(D3D_SDK_VERSION);

	d3dpp.Windowed = TRUE;
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;

	d3d->CreateDevice
	(
		D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp, &d3ddev
	);

	D3DXCreateSprite
	(
		d3ddev, &spriteHandler
	);

	GraphicsHelper::device = d3ddev;
	GraphicsHelper::spriteHandler = spriteHandler;

	D3DXMATRIX worldMatrix;
	D3DXMATRIX projectionMatrix;

	D3DXVECTOR2 scalingCenter(0.0f, 0.0f);
	D3DXVECTOR2 scalingRatio(SCALING_RATIO_X, -SCALING_RATIO_Y);
	D3DXVECTOR2 translationAmount(-SCREEN_WIDTH / 2.0f, +SCREEN_HEIGHT / 2.0f);

	D3DXMatrixIdentity(&worldMatrix);
	D3DXMatrixOrthoLH(&projectionMatrix, SCREEN_WIDTH, -SCREEN_HEIGHT, 0.0f, 1.0f);
	D3DXMatrixTransformation2D(&worldMatrix, &scalingCenter, 0.0f, &scalingRatio, NULL, 0.0f, &translationAmount);

	d3ddev->SetTransform(D3DTS_WORLD, &worldMatrix);
	d3ddev->SetTransform(D3DTS_PROJECTION, &projectionMatrix);
}

void CleanD3D(void)
{
	spriteHandler->Release();
	d3ddev->Release();
	d3d->Release();
}
