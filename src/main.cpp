#include "Bill.h"
#include "Sound.h"
#include "Scene.h"
#include "Input.h"
#include "Camera.h"
#include "Motion.h"
#include "Common.h"

#include <cstdio>    // _snwprintf_s
#include <cwchar>    // wcsrchr

Scene* scene; Input* input; Camera* camera;

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

// Ensures working directory points to executable folder if resources are not in CWD
static const wchar_t* const RESOURCE_SENTINEL = L"Resources\\Fonts\\Font1.json";

static BOOL FileExists(const wchar_t* path)
{
	const DWORD attributes = GetFileAttributesW(path);
	return attributes != INVALID_FILE_ATTRIBUTES
		&& (attributes & FILE_ATTRIBUTE_DIRECTORY) == 0;
}

static void EnsureResourcesAreReachable(void)
{
	if (FileExists(RESOURCE_SENTINEL)) return;

	wchar_t modulePath[MAX_PATH] = {};
	const DWORD moduleLength = GetModuleFileNameW(nullptr, modulePath, ARRAYSIZE(modulePath));
	if (moduleLength == 0 || moduleLength >= ARRAYSIZE(modulePath)) return;

	wchar_t* lastSlash = wcsrchr(modulePath, L'\\');
	if (!lastSlash) return;
	*lastSlash = L'\0';

	wchar_t sentinelPath[MAX_PATH * 2] = {};
	if (_snwprintf_s(sentinelPath, ARRAYSIZE(sentinelPath), _TRUNCATE,
		L"%s\\%s", modulePath, RESOURCE_SENTINEL) < 0) return;

	if (FileExists(sentinelPath))
		SetCurrentDirectoryW(modulePath);
}

// Declares the process DPI-aware, so the back buffer maps 1:1 to physical
// pixels.  Without this Windows treats the window as 96-DPI and bitmap-stretches
// it by the display's scale factor - on a 125% display the 640x600 back buffer
// is resampled to 800x750, which is a NON-INTEGER rescale of point-sampled pixel
// art and undoes the reason GraphicsHelper picks POINT filtering in the first
// place.  With this, the client area really is 640x600 device pixels and
// UpdateViewport's letterboxing handles whatever size the user drags it to.
//
// Resolved at run time because SetProcessDpiAwarenessContext is Windows 10 1703
// and later; older systems fall back to the system-DPI-aware call.
static void EnableDpiAwareness(void)
{
	if (const HMODULE user32 = GetModuleHandleW(L"user32.dll"))
	{
		// PER_MONITOR_AWARE_V2 == (DPI_AWARENESS_CONTEXT)-4.  Spelled through
		// HANDLE so this compiles against SDKs that predate the typedef.
		using SetContextFn = BOOL (WINAPI*)(HANDLE);
		const auto setContext = reinterpret_cast<SetContextFn>(
			reinterpret_cast<void*>(GetProcAddress(user32, "SetProcessDpiAwarenessContext")));

		if (setContext && setContext(reinterpret_cast<HANDLE>(static_cast<INT_PTR>(-4))))
			return;
	}

	SetProcessDPIAware();   // Vista and later; system-DPI aware only.
}

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	EnableDpiAwareness();
	EnsureResourcesAreReachable();

	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX); wc.style = CS_HREDRAW | CS_VREDRAW; wc.lpfnWndProc = WindowProc; wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;	wc.lpszClassName = L"WindowClass";
	RegisterClassEx(&wc);

	// Adjust window rect so client area matches SCREEN_WIDTH x SCREEN_HEIGHT
	RECT windowRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	const DWORD windowStyle = WS_OVERLAPPEDWINDOW;
	AdjustWindowRect(&windowRect, windowStyle, FALSE);

	HWND hWnd = CreateWindowEx
	(
		NULL, L"WindowClass", L"Contra", windowStyle, CW_USEDEFAULT, CW_USEDEFAULT,
		windowRect.right - windowRect.left, windowRect.bottom - windowRect.top,
		NULL, NULL, hInstance, NULL
	);
	if (!hWnd) return 0;

	ShowWindow(hWnd, nCmdShow);

	// Retrieve actual client size or fallback to design resolution
	RECT clientRect = {};
	GetClientRect(hWnd, &clientRect);

	UINT clientWidth  = static_cast<UINT>(clientRect.right  - clientRect.left);
	UINT clientHeight = static_cast<UINT>(clientRect.bottom - clientRect.top);
	if (clientWidth  == 0) clientWidth  = GraphicsHelper::DESIGN_WIDTH;
	if (clientHeight == 0) clientHeight = GraphicsHelper::DESIGN_HEIGHT;

	if (!GraphicsHelper::Init(hWnd, clientWidth, clientHeight))
	{
		GraphicsHelper::Cleanup();
		return 0;
	}

	Sound::create(hWnd);

	scene = new Scene();

	input = new Input
	(
		hInstance, hWnd
	);

	camera = new Camera(new CameraStaticState());

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	// --- Fixed timestep -----------------------------------------------------
	// The game's logic is a hybrid, and that is what forces this design.
	//
	//   * Movement is per FRAME. Motion::CalculateUniformMotion is c + v, with no
	//     dt anywhere, and 24 files rely on it. Every velocity in the game is
	//     tuned in units-per-update.
	//   * Animation frames, weapon fire rates, the GunBossStage1 shoot cadence,
	//     the RifleMan hide timers and the scene transitions are all on the real
	//     clock via GetTickCount64.
	//
	// So the update rate is not a free parameter: it is baked into the tuning of
	// one half of the game and must agree with the wall clock the other half
	// reads. Letting Present's vblank wait set it - as this loop used to - meant
	// the two halves only agreed on a 60 Hz display. On a 120 Hz panel Bill moved
	// at double speed with a normal-speed run cycle; on a 30 Hz one, half speed.
	//
	// Running the logic at a fixed 60 Hz off a real clock, independently of how
	// often we present, restores the relationship both halves were written
	// against, on any display. Fixing it the other way - threading a variable dt
	// through the motion helpers - would mean re-tuning every velocity in the
	// game, and would still leave the frame-tuned constants wrong.
	constexpr double TICK_SECONDS = 1.0 / 60.0;

	// Ceiling on catch-up work per iteration. After a long stall - a debugger
	// breakpoint, a dragged window, a slow stage load - the elapsed time must be
	// absorbed rather than replayed, or the game bursts forward through spawn
	// timers and boss state machines exactly as it did when Present was occluded.
	constexpr int MAX_TICKS_PER_ITERATION = 5;

	LARGE_INTEGER tickFrequency = {};
	LARGE_INTEGER previousCounter = {};
	QueryPerformanceCounter(&previousCounter);
	const BOOL hasClock = QueryPerformanceFrequency(&tickFrequency) && tickFrequency.QuadPart > 0;

	// Primed with one step's worth so the first iteration updates before it draws,
	// as the old loop did. Starting at zero would render the opening frame with
	// the identity view matrix Init leaves behind, because the camera is only
	// positioned inside a logic step - a visibly mis-framed first frame.
	double tickAccumulator = TICK_SECONDS;

	BOOL running = TRUE;
	while (running)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) { running = FALSE; break; }

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		if (!running) break;

		// Real time since the previous iteration. Clamped at zero because a
		// counter that appears to run backwards (and QPC has historically done so
		// across cores on some hardware) must not rewind the accumulator.
		LARGE_INTEGER currentCounter = {};
		QueryPerformanceCounter(&currentCounter);
		double elapsedSeconds = hasClock
			? static_cast<double>(currentCounter.QuadPart - previousCounter.QuadPart)
			  / static_cast<double>(tickFrequency.QuadPart)
			: TICK_SECONDS;   // no usable clock: fall back to one tick per frame
		previousCounter = currentCounter;

		if (elapsedSeconds < 0.0) elapsedSeconds = 0.0;
		tickAccumulator += elapsedSeconds;

		int ticksRun = 0;
		while (tickAccumulator >= TICK_SECONDS && ticksRun < MAX_TICKS_PER_ITERATION)
		{
			// One logic step. Input is captured per step, not per iteration, so a
			// step always sees the same input the state machines expect - and
			// DirectInput's relative axes (the zoom wheel) report their delta only
			// to the first read, so a catch-up step cannot double-apply them.
			input->Capture();

			scene->HandleInput(*input);
			scene->Update();
			if (scene->stageIsReady)
			{
				scene->stage->CheckResolveClearCollision();
			}

			// VIEW stage: pass camera view matrix to renderer
			if (scene->stageIsReady)
			{
				scene->stage->GetCamera()->HandleInput(*input);
				scene->stage->GetCamera()->Capture
				(
					scene->stage->GetBill()->GetX(),
					scene->stage->GetBill()->GetY()
				);
				GraphicsHelper::SetViewMatrix(scene->stage->GetCamera()->GetViewMatrix());
			}
			else
			{
				camera->HandleInput(*input);
				camera->Capture
				(
					IN_GAME_SCREEN_W / 2.0f,
					IN_GAME_SCREEN_H / 2.0f
				);
				GraphicsHelper::SetViewMatrix(camera->GetViewMatrix());
			}

			tickAccumulator -= TICK_SECONDS;
			++ticksRun;
		}

		// Hit the ceiling with a WHOLE step still owed, so there is a real
		// backlog. Drop it: kept, it would only grow and turn every later
		// iteration into a burst of catch-up.
		//
		// The second test is what keeps this from stealing time. Hitting the
		// ceiling does not by itself mean anything is owed - an iteration that
		// arrives 5.4 steps late runs its five and leaves 0.4 of a step, which
		// is not a backlog but 6.7 ms of real elapsed time. Zeroing on ticksRun
		// alone discarded that, so on a machine where five steps per iteration
		// is the norm the game ran steadily slow.
		if (ticksRun == MAX_TICKS_PER_ITERATION && tickAccumulator >= TICK_SECONDS)
			tickAccumulator = 0.0;

		// Minimised or fully covered, as of the previous Present. Nothing this
		// frame draws could reach the screen, so skip the drawing itself rather
		// than building a frame for Present to throw away - the game state above
		// still advances at 60 Hz either way. The Sleep is what stops the loop
		// pinning a core, since an occluded Present would not have waited for
		// vblank to pace it.
		if (GraphicsHelper::IsOccluded())
		{
			Sleep(16);
			continue;
		}

		// Present every iteration even when no step ran. On a display faster than
		// 60 Hz that redraws an unchanged frame, which is what showing a 60 Hz
		// game on a 120 Hz panel means; more importantly it keeps Present's vblank
		// wait throttling the loop instead of letting it spin.
		GraphicsHelper::Clear(0.0f, 0.0f, 0.0f, 1.0f);
		GraphicsHelper::Begin();

		scene->Render();

		GraphicsHelper::End();

		switch (GraphicsHelper::Present())
		{
		case GraphicsHelper::PresentResult::Presented:
			// Present waited for vblank, which paces the redraw. Game speed does
			// not depend on that any more - the accumulator above owns it.
			break;

		case GraphicsHelper::PresentResult::Occluded:
			// The frame we just built was the one that DISCOVERED the occlusion;
			// from here the check above takes over and skips the drawing.
			Sleep(16);
			break;

		case GraphicsHelper::PresentResult::DeviceLost:
			// Nothing can be drawn again and GraphicsHelper has already told the
			// player why; carrying on would just run the game behind a dead window.
			running = FALSE;
			break;
		}
	}

	Sound::getInstance()->cleanUp();
	Destroy(input);
	Destroy(scene);
	Destroy(camera);
	GraphicsHelper::Cleanup();

	return (int)msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{

	case WM_SIZE:
		if (wParam != SIZE_MINIMIZED)
			GraphicsHelper::OnResize(LOWORD(lParam), HIWORD(lParam));
		return 0;

#ifdef WM_DPICHANGED
	// Dragged to a monitor with a different scale factor.  A PerMonitorV2
	// process is expected to adopt the rectangle Windows suggests; the WM_SIZE
	// that follows re-letterboxes the back buffer into it.
	case WM_DPICHANGED:
	{
		const RECT* const suggested = reinterpret_cast<const RECT*>(lParam);
		if (suggested)
			SetWindowPos(hWnd, nullptr,
				suggested->left, suggested->top,
				suggested->right  - suggested->left,
				suggested->bottom - suggested->top,
				SWP_NOZORDER | SWP_NOACTIVATE);
		return 0;
	}
#endif

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;

	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}
