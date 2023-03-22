#include "Common.h"
#include "GraphicsHelper.h"
#include "GraphicsDatabase.h"
#include "Bill.h"
#include "Input.h"

Input* input;

// global declarations
LPDIRECT3D9 d3d;            // the pointer to our Direct3D interface
LPDIRECT3DDEVICE9 d3ddev;   // the pointer to the device class

// function prototypes
void initD3D(HWND hWnd);    // sets up and initializes Direct3D
void render_frame(void);    // renders a single frame
void cleanD3D(void);        // closes Direct3D and releases memory

// fucntion prototypes for sprite
void initSprite();
void drawSprite();

// the WindowProc function prototype
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

// the entry point for any Windows program
int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow)
{
	HWND hWnd;          // the handle for the window, filled by a function
	WNDCLASSEX wc;      // this struct holds information for the window class

	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = L"WindowClass";

	// register the window class
	RegisterClassEx(&wc);

	// create the window and use the result as the handle
	hWnd = CreateWindowEx(NULL,
		L"WindowClass", 					// name of the window class
		L"Load Sprite Direct3D Program",  	// title of the window
		WS_OVERLAPPEDWINDOW,   				// window style
		0, 0, 							// x, y - position of the window
		SCREEN_WIDTH, SCREEN_HEIGHT, 							// width, height of the window
		NULL,   							// we have no parent window, NULL
		NULL,								// we aren't using menus, NULL
		hInstance, 							// application handle
		NULL);  							// used with multiple windows, NULL

	// display the window on the screen
	ShowWindow(hWnd, nCmdShow);

	// set up and initialize Direct3D
	initD3D(hWnd);

	input = new Input(hInstance, hWnd);

	// set up and initialize Sprite
	initSprite();

	// this struct holds Windows event messages
	MSG msg;

	// wait for the next message in the queue, store the result in 'msg'
	while (TRUE)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// translate keystroke messages into the right format
			TranslateMessage(&msg);
			// send the message to the WindowProc function
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT)
			break;

		render_frame();
	}

	// clean up DirectX and COM
	cleanD3D();
	delete input;
	input = NULL;

	// return this part of the WM_QUIT message to Windows
	return msg.wParam;
}

/////////////////////////////////////////////////////////////////////
// load Sprite here
/////////////////////////////////////////////////////////////////////
LPD3DXSPRITE spriteHandler;
Bill bill;
void initSprite()
{
	D3DXCreateSprite(d3ddev, &spriteHandler);
	GraphicsHelper::device = d3ddev;
	GraphicsHelper::spriteHandler = spriteHandler;

	GraphicsDatabase::textures.insert({ BILL, GraphicsHelper::CreateTexture(L"Resources\\Textures\\MainCharacter2.bmp") });

	GraphicsDatabase::sprites.insert({ BILL_NORMAL_01, GraphicsHelper::CreateSprite(30 ,1  ,25 ,65 ,BILL) });
	GraphicsDatabase::sprites.insert({ BILL_NORMAL_02, GraphicsHelper::CreateSprite(30 ,26 ,50 ,65 ,BILL) });

	GraphicsDatabase::sprites.insert({ BILL_RUN_01, GraphicsHelper::CreateSprite(66 ,2  ,23 ,102,BILL) });
	GraphicsDatabase::sprites.insert({ BILL_RUN_02, GraphicsHelper::CreateSprite(66 ,24 ,41 ,102,BILL) });
	GraphicsDatabase::sprites.insert({ BILL_RUN_03, GraphicsHelper::CreateSprite(66 ,42 ,61 ,102,BILL) });
	GraphicsDatabase::sprites.insert({ BILL_RUN_04, GraphicsHelper::CreateSprite(66 ,62 ,83 ,102,BILL) });
	GraphicsDatabase::sprites.insert({ BILL_RUN_05, GraphicsHelper::CreateSprite(66 ,84 ,101,102,BILL) });
	GraphicsDatabase::sprites.insert({ BILL_RUN_06, GraphicsHelper::CreateSprite(66 ,102,122,102,BILL) });

	GraphicsDatabase::sprites.insert({ BILL_JUMP_01, GraphicsHelper::CreateSprite(44 ,117,134,65 ,BILL) });
	GraphicsDatabase::sprites.insert({ BILL_JUMP_02, GraphicsHelper::CreateSprite(44 ,135,155,65 ,BILL) });
	GraphicsDatabase::sprites.insert({ BILL_JUMP_03, GraphicsHelper::CreateSprite(44 ,156,173,65 ,BILL) });
	GraphicsDatabase::sprites.insert({ BILL_JUMP_04, GraphicsHelper::CreateSprite(44 ,174,194,65 ,BILL) });

	GraphicsDatabase::sprites.insert({ BILL_LAYDOWN_01, GraphicsHelper::CreateSprite(30 ,83 ,116,65 ,BILL) });

	GraphicsDatabase::animations.insert
	({ BILL_NORMAL, GraphicsHelper::CreateAnimation
	(150,
		{
			{BILL_NORMAL_01,0},
			{BILL_NORMAL_02,0},
		}
	) });

	GraphicsDatabase::animations.insert
	({ BILL_RUN, GraphicsHelper::CreateAnimation
	(150,
		{
			{BILL_RUN_01,0},
			{BILL_RUN_02,0},
			{BILL_RUN_03,0},
			{BILL_RUN_04,0},
			{BILL_RUN_05,0},
			{BILL_RUN_06,0},
		}
	) });

	GraphicsDatabase::animations.insert
	({ BILL_JUMP, GraphicsHelper::CreateAnimation
	(150,
		{
			{BILL_JUMP_01,0},
			{BILL_JUMP_02,0},
			{BILL_JUMP_03,0},
			{BILL_JUMP_04,0},
		}
	) });

	GraphicsDatabase::animations.insert
	({ BILL_LAYDOWN, GraphicsHelper::CreateAnimation
	(150,
		{
			{BILL_LAYDOWN_01,0},
		}
	) });

}

void drawSprite()
{
	bill.HandleInput(*input);
	bill.Update();
	bill.Render();
	//GraphicsHelper::DrawSprite(GraphicsDatabase::sprites[100], D3DXVECTOR3(100, 50, 0));
}
///////////////////////////////////////////////////////////////////

// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// sort through and find what code to run for the message given
	switch (message)
	{
		// this message is read when the window is closed
	case WM_DESTROY:
	{
		PostQuitMessage(0);        // close the application entirely

		return 0;
	} break;
	case WM_CLOSE:
	{
		PostQuitMessage(0);        // close the application entirely

		return 0;
	} break;
	}
	// Handle any messages the switch statement didn't
	return DefWindowProc(hWnd, message, wParam, lParam);
}


// this function initializes and prepares Direct3D for use
void initD3D(HWND hWnd)
{
	d3d = Direct3DCreate9(D3D_SDK_VERSION);    	// create the Direct3D interface

	D3DPRESENT_PARAMETERS d3dpp;    			// create a struct to hold various device information

	ZeroMemory(&d3dpp, sizeof(d3dpp));    		// clear out the struct for use // fill null
	d3dpp.Windowed = TRUE;    					// program windowed, not fullscreen
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;   // discard old frames
	d3dpp.hDeviceWindow = hWnd;    				// set the window to be used by Direct3D

	// create a device class using this information and the info from the d3dpp stuct
	d3d->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&d3ddev);
}

// this is the function used to render a single frame
void render_frame(void)
{
	input->Capture();
	// clear the window to a deep blue
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 0);

	d3ddev->BeginScene();

	drawSprite();	// draw sprite

	d3ddev->EndScene();

	// displays the created frame on the screen
	// translate backbuffer to frontbuffer
	d3ddev->Present(NULL, NULL, NULL, NULL);
}

// this is the function that cleans up Direct3D and COM
void cleanD3D(void)
{
	spriteHandler->Release();
	d3ddev->Release();    // close and release the 3D device
	d3d->Release();       // close and release Direct3D
}