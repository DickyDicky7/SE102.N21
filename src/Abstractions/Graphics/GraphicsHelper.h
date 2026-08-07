#pragma once

#include "Common.h"
#include "GraphicsDatabase.h"

// GraphicsHelper - Direct3D 11 rendering back end for the 2-D sprite engine.
// Features:
//   * Explicit LOCAL -> WORLD -> VIEW -> CLIP transform chain.
//   * Instanced sprite batching, streamed through a ring buffer and broken only
//     when the texture changes.  Nothing is sorted: blending is enabled and the
//     game relies on later sprites covering earlier ones, so grouping draws by
//     texture would change what ends up on screen.
//   * Build-time shader compilation (with runtime fallback).
//   * Texture loading via WIC (BMP/PNG/JPEG, etc.).
class GraphicsHelper
{

public:

	// Device objects (exposed for compatibility)
	static ID3D11Device*        device;
	static ID3D11DeviceContext* context;
	static IDXGISwapChain*      swapChain;

	// Outcome of a frame presentation.
	//   Presented  - the frame reached the screen and Present waited for vblank,
	//                so it has already paced this frame.
	//   Occluded   - minimised or fully covered. An occluded flip-model Present
	//                returns WITHOUT waiting, so the caller must pace the frame
	//                itself or spin through the level's timers.
	//   DeviceLost - the device is gone (TDR, driver update, adapter removed) or
	//                the back buffer could not be rebuilt. Nothing will ever be
	//                drawn again; the failure has been reported to the user, and
	//                the caller is expected to shut down.
	enum class PresentResult { Presented, Occluded, DeviceLost };

	// Lifecycle
	static BOOL Init    (HWND hWnd, UINT clientWidth, UINT clientHeight);
	static void Cleanup (void);

	static void Clear   (FLOAT r, FLOAT g, FLOAT b, FLOAT a);
	static void Begin   (void);
	static void End     (void);

	static PresentResult Present (void);

	// TRUE while the window is minimised or fully covered, so the caller can
	// skip the frame's drawing entirely: nothing would reach the screen, and an
	// occluded Present does not wait for vblank either way.  Cheap - it only
	// asks DXGI (via a DXGI_PRESENT_TEST) once Present has actually reported
	// occlusion, and answers FALSE outright the rest of the time.
	static BOOL IsOccluded(void);

	// Recreates back buffer and viewport on window resize
	static void OnResize(UINT clientWidth, UINT clientHeight);

	// Camera setup
	static void SetViewMatrix(const D3DMATRIX& view);

	// Resource creation
	static SPRITE    CreateSprite   (INT top, INT left, INT right, INT bottom, DIRECTION, TEXTURE_ID);
	static ANIMATION CreateAnimation(DEFAULT_TIME, std::vector<std::tuple<SPRITE_ID, TIME>>);

	static void InsertTexure   (TEXTURE_ID, LPCWSTR);
	static void InsertSprite   (SPRITE_ID, INT top, INT left, INT right, INT bottom, DIRECTION, TEXTURE_ID);
	static void InsertAnimation(ANIMATION_ID, DEFAULT_TIME, std::vector<std::tuple<SPRITE_ID, TIME>>);

	// Drawing
	// Draws sprite anchored at position, with optional mirroring and rotation.
	// Taken by reference: SPRITE holds a TEXTURE_ID, which is a variant over ~20
	// enum types, and this runs once per sprite per frame.
	static void DrawSprite(const SPRITE& sprite, D3DXVECTOR3 position, DIRECTION movingDirection, FLOAT angle);

	// Axis-aligned-then-rotated wireframe box in world space for hitbox debugging
	static void DrawBox(FLOAT left, FLOAT bottom, FLOAT right, FLOAT top,
	                    FLOAT angle, D3DCOLOR colour,
	                    FLOAT pivotX = 0.0f, FLOAT pivotY = 0.0f);

	// Design resolution and letterboxing config
	static constexpr UINT DESIGN_WIDTH  = SCREEN_WIDTH;
	static constexpr UINT DESIGN_HEIGHT = SCREEN_HEIGHT;

private:

	// Hands back a TEXTURE holding an SRV reference that only Cleanup's sweep
	// over GraphicsDatabase::textures ever releases, so the caller MUST be the
	// one that stores it there.  InsertTexure is that caller; keeping this
	// private means a second one cannot appear and quietly leak the view.
	static TEXTURE CreateTexture(LPCWSTR textureFilePath);

};
