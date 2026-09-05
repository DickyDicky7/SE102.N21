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

	// Constants::Rgba deliberately knows nothing about DirectXMath, so the two
	// are bridged here - the one place that already depends on both - rather
	// than by a free function at global scope in whichever header needed it.
	static DirectX::XMFLOAT4 ToXMFloat4(const Constants::Rgba& colour)
	{
		return DirectX::XMFLOAT4(colour.r, colour.g, colour.b, colour.a);
	}

	// Lifecycle
	static bool Init    (HWND hWnd, UINT clientWidth, UINT clientHeight);
	static void Cleanup (void);

	static void Clear   (float r, float g, float b, float a);
	static void Begin   (void);
	static void End     (void);

	static PresentResult Present (void);

	// true while the window is minimised or fully covered, so the caller can
	// skip the frame's drawing entirely: nothing would reach the screen, and an
	// occluded Present does not wait for vblank either way.  Cheap - it only
	// asks DXGI (via a DXGI_PRESENT_TEST) once Present has actually reported
	// occlusion, and answers false outright the rest of the time.
	static bool IsOccluded(void);

	// Recreates back buffer and viewport on window resize
	static void OnResize(UINT clientWidth, UINT clientHeight);

	// Camera setup
	static void SetViewMatrix(const D3DMATRIX& view);

	// Resource creation
	static SPRITE    CreateSprite   (int top, int left, int right, int bottom, DIRECTION spriteDirection, const TEXTURE_ID& textureId);
	static ANIMATION CreateAnimation(TIME defaultTime, std::vector<std::tuple<SPRITE_ID, TIME>> frames);

	static void InsertTexture  (const TEXTURE_ID& textureId, LPCWSTR textureFilePath);
	static void InsertSprite   (const SPRITE_ID& spriteId, int top, int left, int right, int bottom, DIRECTION spriteDirection, const TEXTURE_ID& textureId);
	static void InsertAnimation(const ANIMATION_ID& animationId, TIME defaultTime, std::vector<std::tuple<SPRITE_ID, TIME>> frames);

	// Drawing
	// Draws sprite anchored at position, with optional mirroring and rotation.
	// Taken by reference: SPRITE holds a TEXTURE_ID, which is a variant over ~20
	// enum types, and this runs once per sprite per frame.
	static void DrawSprite(const SPRITE& sprite, D3DXVECTOR3 position, DIRECTION movingDirection, float angle);

	// Draws a glowing particle using a custom programmatic texture and additive blending.
	static void DrawParticle(D3DXVECTOR3 position, float size, DirectX::XMFLOAT4 tint);

	// Axis-aligned-then-rotated wireframe box in world space for hitbox debugging
	static void DrawBox(float left, float bottom, float right, float top,
	                    float angle, D3DCOLOR colour,
	                    float pivotX = 0.0f, float pivotY = 0.0f);

	// Design resolution and letterboxing config
	static constexpr UINT DESIGN_WIDTH  = Constants::Screen::WIDTH;
	static constexpr UINT DESIGN_HEIGHT = Constants::Screen::HEIGHT;

private:

	// Hands back a TEXTURE holding an SRV reference that only Cleanup's sweep
	// over GraphicsDatabase::textures ever releases, so the caller MUST be the
	// one that stores it there.  InsertTexture is that caller; keeping this
	// private means a second one cannot appear and quietly leak the view.
	static TEXTURE CreateTexture(LPCWSTR textureFilePath);

};
