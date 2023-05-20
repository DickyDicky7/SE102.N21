﻿#include "Input.h"
#include "Motion.h"
#include "Camera.h"
#include "Common.h"
#include "tileson.hpp"
#include "QuadTreeContainer.h"

#include "Bill.h"
#include "Bullet.h"
#include "Falcon.h"
#include "Soldier.h"
#include "WallTurret.h"
#include "BossStage3.h"
#include "ScubaSoldier.h"
#include "TestingEntity.h"
#include "RifleManStanding.h"
#include "AirCraft.h"
#include "RifleManHideOnBush.h"

Input* input;
Camera* camera;
LPDIRECT3D9 d3d;
LPDIRECT3DDEVICE9 d3ddev;
LPD3DXSPRITE spriteHandler;

Bill bill;
Soldier soldier;
WallTurret wallTurret;
Falcon falcon;
BossStage3 bossStage3;
ScubaSoldier scubaSoldier;
AirCraft airCraft;
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

	tson::Tileson tile;
	std::unique_ptr<tson::Map> map = tile.parse(fs::path("Resources/Maps/stage1.json"));
	tson::Layer* tileLayer = map.get()->getLayer("BackGroundLayer");
	if (tileLayer->getType() == tson::LayerType::TileLayer)
	{
		for (auto& [pos,obj] : tileLayer->getTileObjects())
		{
			_RPT1(0,"%f, %f\n", obj.getPosition().x, obj.getPosition().y);
		}
	}

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

	QuadTreeContainer quadTreeContainer = QuadTreeContainer(
		QuadTreeRect::QTRect({
			{0, 0},
			{SCREEN_WIDTH / SCALING_RATIO_X, SCREEN_HEIGHT / SCALING_RATIO_Y}
			})
	);

	quadTreeContainer.Insert(&bill);
	quadTreeContainer.Insert(&soldier);
	quadTreeContainer.Insert(&falcon);
	quadTreeContainer.Insert(&wallTurret);
	quadTreeContainer.Insert(&bossStage3);
	quadTreeContainer.Insert(&airCraft);
	quadTreeContainer.Insert(&scubaSoldier);
	quadTreeContainer.Insert(&rifleManStanding);
	quadTreeContainer.Insert(&rifleManHideOnBush);

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
		falcon.HandleInput(*input);
		airCraft.HandleInput(*input);
		bossStage3.HandleInput(*input);
		scubaSoldier.HandleInput(*input);

		bill.Update();

		for (auto it = bill.GetBullets().begin(); it != bill.GetBullets().end(); it++)
		{
			(*it)->Update();
			if ((*it)->GetX() > 200.0f)
			{
				Destroy((*it));
				//bill.GetBullets().erase(it);
			}
		}
		bill.GetBullets().remove_if([](Bullet* bullet) { return bullet == NULL; });

		soldier.Update();
		falcon.Update();
		wallTurret.Update();
		bossStage3.Update();
		airCraft.Update();
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
		bill.CollideWith(&falcon);
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



		// vẽ chuyển động ====================================================
		auto poo = Motion::CalculateOscillatoryMotion(pio);
		pio.t = poo.t;
		GraphicsHelper::DrawSprite
		(GraphicsDatabase::sprites[SOLDIER_SPRITE_ID::RUN_01], D3DXVECTOR3(x, poo.c, 0.0f), DIRECTION::RIGHT, 45.0f);
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
		(GraphicsDatabase::sprites[BILL_SPRITE_ID::RUN_01], D3DXVECTOR3(pip.x, pip.y, 0.0f), DIRECTION::RIGHT, -45.0f);


		auto poc = Motion::CalculateUniformCircularMotion(pic);
		pic.ω = poc.ω;
		GraphicsHelper::DrawSprite
		(GraphicsDatabase::sprites[BILL_SPRITE_ID::RUN_02], D3DXVECTOR3(poc.x, poc.y, 0.0f), DIRECTION::RIGHT, angle);
		angle += dAngle;
		//



		bill.Render();

		for (auto& bullet : bill.GetBullets())
			bullet->Render();

		soldier.Render();
		falcon.Render();
		wallTurret.Render();
		bossStage3.Render();
		scubaSoldier.Render();
		airCraft.Render();
		rifleManStanding.Render();
		rifleManHideOnBush.Render();


		for (auto it = quadTreeContainer.begin(); it != quadTreeContainer.end(); it++)
		{
			quadTreeContainer.Relocate(it);
		}

		std::list<Entity*> result = quadTreeContainer.GetCollisionWithTarget(&bill);

		for (auto it = result.begin(); it != result.end(); it++)
		{
			if (*it != &bill)
			{
				(*it)->LogName();
			}
		}

		d3ddev->EndScene();
		d3ddev->Present(NULL, NULL, NULL, NULL);

		//Should disable if you do not want to debug, this line of code causes lag
		//_RPT1(0, "W: %f ; H: %f\n", bill.GetW(), bill.GetH());
		result.clear();
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

	Bullet* bullet = new Bullet();
	bullet->LoadTextures();
	bullet->LoadSprites();
	bullet->LoadAnimations();
	Destroy(bullet);

	soldier.LoadTextures();
	soldier.LoadSprites();
	soldier.LoadAnimations();
	soldier.SetTarget(&bill);

	falcon.LoadTextures();
	falcon.LoadSprites();
	falcon.LoadAnimations();
	falcon.SetTarget(&bill);

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
	
	airCraft.LoadTextures();
	airCraft.LoadSprites();
	airCraft.LoadAnimations();
	airCraft.SetTarget(&bill);

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
