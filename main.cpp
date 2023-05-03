#include "Input.h"
#include "Motion.h"
#include "Camera.h"
#include "Common.h"

#include "Bill.h"
#include "Soldier.h"
#include "WallTurret.h"
#include "BossStage3.h"
#include "ScubaSoldier.h"
#include "TestingEntity.h"
#include "RifleManStanding.h"
#include "RifleManHideOnBush.h"

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
RifleManStanding rifleManStanding;
RifleManHideOnBush rifleManHideOnBush;

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
	LoadAssets();

	input = new Input
	(
		hInstance, hWnd
	);
	camera = new Camera
	(
		/* input, */
		new CameraMovingForwardState()
	);


	//
	FLOAT x = 050.0f;
	FLOAT y = 100.0f;
	FLOAT vx = 5.0f;
	FLOAT vy = 5.0f;


	FLOAT y0 = y;
	FLOAT t = 0.0f;
	FLOAT dt = 0.05f;
	FLOAT T = 2.5f;
	FLOAT A = 50.0f;
	FLOAT φ = 0.0f;
	Motion::OscillatoryMotionInputParameters pio{ y0, t, dt, T, A, φ };


	FLOAT v0 = 10.0f;
	FLOAT θ = 80.0f;
	dt = 0.05f;
	Motion::ProjectileMotionInputParameters pip{ x, y, v0, θ, t, dt };


	FLOAT r = 50.0f;
	FLOAT ω = 0.0f;
	FLOAT dω = 5.0f;
	FLOAT xO = 100.0f;
	FLOAT yO = 100.0f;
	FLOAT angle = 0.0f;
	FLOAT dAngle = 10.0f;
	Motion::UniformCircularMotionInputParameters pic{ r, ω, dω, xO, yO };
	//


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
		wallTurret.Update();
		bossStage3.Update();
		scubaSoldier.Update();
		rifleManStanding.Update();
		rifleManHideOnBush.Update();




		//auto ry = bill.AABBSweepY(&scubaSoldier);
		//if (ry.isCollided)
		//{
		//	if (ry.normalY != 0)
		//		bill.SetY(bill.GetY() + ry.enTime * bill.GetVY());
		//}
		//auto rx = bill.AABBSweepX(&scubaSoldier);
		//if (rx.isCollided)
		//{
		//	if (rx.normalX != 0)
		//		bill.SetX(bill.GetX() + rx.enTime * bill.GetVX());
		//}
		bill.CollideWith(&soldier);
		bill.CollideWith(&bossStage3);
		bill.CollideWith(&wallTurret);
		//_RPT1(0, "entryTime: %f, exitTime: %f, normalX: %f, normalY: %f\ncollided: %d\n\n", ry.enTime, ry.exTime, ry.normalX, ry.normalY, ry.isCollided);
		//_RPT1(0, "X collided: %d, Y collided: %d, etX: %f, etY: %f\n", rx.isCollided, ry.isCollided, rx.enTime, ry.enTime);
		//_RPT1(0, "cpX: %f, cpY: %f\n", rx.contactX, rx.contactY);




		//if (bill.GetY() <= 0)
		//{
		//	camera->Capture
		//	(
		//		bill.GetX(), bill.GetY(),
		//		bill.GetVX(), bill.GetVY()
		//	);
		//}
		camera->HandleInput(*input);
		camera->Capture(bill.GetX(), bill.GetY());

		d3ddev->SetTransform(D3DTS_VIEW, &camera->GetViewMatrix());
		d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 0);
		d3ddev->BeginScene();



		//
		auto poo = Motion::CalculateOscillatoryMotion(pio);
		pio.t = poo.t;
		GraphicsHelper::DrawSprite
		(GraphicsDatabase::sprites[BILL_SPRITE_ID::NORMAL_01], D3DXVECTOR3(x, poo.c, 0.0f), DIRECTION::RIGHT, 45.0f);
		x += vx;
		if (x >= 500.0f || x <= 0.0f) vx = -vx;


		if (pip.y < 0.0f)
		{
			pip.y = 0.0f;
			pip.t = 0.0f;
			if (pip.x >= 500.0f) pip.θ += 20.0f;
			if (pip.x <= 0.0f) pip.θ -= 20.0f;
		}
		else
		{
			auto pop = Motion::CalculateProjectileMotion(pip);
			pip.x = pop.x;
			pip.y = pop.y;
			pip.t = pop.t;
		}
		GraphicsHelper::DrawSprite
		(GraphicsDatabase::sprites[BILL_SPRITE_ID::NORMAL_01], D3DXVECTOR3(pip.x, pip.y, 0.0f), DIRECTION::RIGHT, -45.0f);


		auto poc = Motion::CalculateUniformCircularMotion(pic);
		pic.ω = poc.ω;
		GraphicsHelper::DrawSprite
		(GraphicsDatabase::sprites[BILL_SPRITE_ID::NORMAL_01], D3DXVECTOR3(poc.x, poc.y, 0.0f), DIRECTION::RIGHT, angle);
		angle += dAngle;
		//



		bill.Render();
		soldier.Render();
		wallTurret.Render();
		bossStage3.Render();
		scubaSoldier.Render();
		rifleManStanding.Render();
		rifleManHideOnBush.Render();

		d3ddev->EndScene();
		d3ddev->Present(NULL, NULL, NULL, NULL);

		//Should disable if you do not want to debug, this line of code causes lag
		//_RPT1(0, "W: %f ; H: %f\n", bill.GetW(), bill.GetH());
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

	wallTurret.LoadTextures();
	wallTurret.LoadSprites();
	wallTurret.LoadAnimations();
	wallTurret.SetTarget(&bill);

	bossStage3.LoadTextures();
	bossStage3.LoadSprites();
	bossStage3.LoadAnimations();
	bossStage3.SetTarget(&bill);

	scubaSoldier.LoadTextures();
	scubaSoldier.LoadSprites();
	scubaSoldier.LoadAnimations();
	scubaSoldier.SetTarget(&bill);

	rifleManStanding.LoadTextures();
	rifleManStanding.LoadSprites();
	rifleManStanding.LoadAnimations();
	rifleManStanding.SetTarget(&bill);

	rifleManHideOnBush.LoadTextures();
	rifleManHideOnBush.LoadSprites();
	rifleManHideOnBush.LoadAnimations();
	rifleManHideOnBush.SetTarget(&bill);
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





	// Set up anti-aliasing
	d3dpp.MultiSampleQuality = 0;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_8_SAMPLES;





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
	D3DXVECTOR2 flippingRatio(+1.0f, -1.0f);
	D3DXVECTOR2 flippingCenter(+0.0f, +0.0f);

	D3DXMatrixIdentity(&worldMatrix);
	D3DXMatrixOrthoLH(&projectionMatrix, SCREEN_WIDTH, -SCREEN_HEIGHT, 0.0f, 1.0f);
	D3DXMatrixTransformation2D(&worldMatrix, &flippingCenter, 0.0f, &flippingRatio, NULL, 0.0f, NULL);

	d3ddev->SetTransform(D3DTS_WORLD, &worldMatrix);
	d3ddev->SetTransform(D3DTS_PROJECTION, &projectionMatrix);
}

void CleanD3D(void)
{
	spriteHandler->Release();
	d3ddev->Release();
	d3d->Release();
}
