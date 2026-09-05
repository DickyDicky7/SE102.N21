#include "GraphicsHelper.h"

#include <dxgi1_2.h>
#include <d3dcompiler.h>
#include <wincodec.h>
#include <wrl/client.h>

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <vector>

using Microsoft::WRL::ComPtr;
using namespace DirectX;



// ===========================================================================
// Exposed device objects
// ===========================================================================
ID3D11Device*        GraphicsHelper::device    = nullptr;
ID3D11DeviceContext* GraphicsHelper::context   = nullptr;
IDXGISwapChain*      GraphicsHelper::swapChain = nullptr;

// ===========================================================================
// Internal state
// ===========================================================================
namespace
{
	// --- GPU data layouts -------------------------------------------------

	// Per-vertex, slot 0: Static unit quad in local space anchored at bottom-center.
	struct QuadVertex
	{
		XMFLOAT2 localPosition;
		XMFLOAT2 cornerWeight;   // (0,0) = top-left .. (1,1) = bottom-right
	};

	// Per-instance, slot 1: one sprite.
	struct SpriteInstance
	{
		XMFLOAT4X4 world;        // LOCAL -> WORLD (row-vector convention)
		XMFLOAT4   sourceRect;   // (u0, v0, u1, v1) inside the atlas
		XMFLOAT4   tint;
	};

	// Constant buffer b0, shared by the sprite and debug shaders.
	struct CameraConstants
	{
		XMFLOAT4X4 view;
		XMFLOAT4X4 projection;
	};

	struct DebugVertex
	{
		XMFLOAT3 worldPosition;
		XMFLOAT4 colour;
	};

	constexpr UINT INSTANCE_RING_CAPACITY = Constants::Graphics::INSTANCE_RING_CAPACITY;
	constexpr UINT MAX_SPRITES_PER_BATCH  = Constants::Graphics::MAX_SPRITES_PER_BATCH;
	constexpr UINT MAX_DEBUG_VERTICES     = Constants::Graphics::MAX_DEBUG_VERTICES;

	static_assert(MAX_SPRITES_PER_BATCH <= INSTANCE_RING_CAPACITY,
	              "a single batch has to fit in the ring");

	struct PostProcessConstants
	{
		float time;
		float frameCount;
		float canvasSize[2];
		float texelSize[2];
		float padding[2];
		float mousePosition[4];
	};

	// --- Pipeline objects -------------------------------------------------
	ComPtr<ID3D11RenderTargetView> g_renderTargetView;

	ComPtr<ID3D11Texture2D>          g_offscreenTexture;
	ComPtr<ID3D11RenderTargetView>    g_offscreenRenderTargetView;
	ComPtr<ID3D11ShaderResourceView>  g_offscreenShaderResourceView;

	ComPtr<ID3D11VertexShader>        g_postProcessVS;
	ComPtr<ID3D11PixelShader>         g_postProcessPS;
	ComPtr<ID3D11Buffer>              g_postProcessConstantBuffer;
	ComPtr<ID3D11ShaderResourceView>  g_lutShaderResourceView;

	D3D11_VIEWPORT                    g_letterboxViewport = {};

	float                             g_postProcessTime = 0.0f;
	float                             g_postProcessFrameCount = 0.0f;
	LARGE_INTEGER                     g_timeStart = {};
	LARGE_INTEGER                     g_timeFrequency = {};

	ComPtr<ID3D11VertexShader>  g_spriteVS;
	ComPtr<ID3D11PixelShader>   g_spritePS;
	ComPtr<ID3D11InputLayout>   g_spriteInputLayout;
	ComPtr<ID3D11Buffer>        g_quadVertexBuffer;    // immutable, 4 vertices
	ComPtr<ID3D11Buffer>        g_quadIndexBuffer;     // immutable, 6 indices
	ComPtr<ID3D11Buffer>        g_instanceBuffer;      // dynamic

	ComPtr<ID3D11VertexShader>  g_debugVS;
	ComPtr<ID3D11PixelShader>   g_debugPS;
	ComPtr<ID3D11InputLayout>   g_debugInputLayout;
	ComPtr<ID3D11Buffer>        g_debugVertexBuffer;   // dynamic

	ComPtr<ID3D11Buffer>             g_cameraBuffer;   // dynamic
	ComPtr<ID3D11SamplerState>       g_samplerState;
	ComPtr<ID3D11SamplerState>       g_linearSamplerState;
	ComPtr<ID3D11BlendState>         g_blendState;
	ComPtr<ID3D11BlendState>         g_additiveBlendState;
	ComPtr<ID3D11ShaderResourceView>  g_particleSRV;
	ComPtr<ID3D11RasterizerState>    g_rasterizerState;
	ComPtr<ID3D11DepthStencilState>  g_depthStencilState;

	ComPtr<IWICImagingFactory> g_wicFactory;
	bool g_comInitialised = false;

	// Set once the pipeline can no longer draw: the device was lost, or the back
	// buffer could not be rebuilt after a resize.  Without this the game would
	// keep looping over a window that never updates again - Clear and Begin
	// silently no-op with no render target view, and Present on a swap chain
	// whose buffers were never recreated still succeeds.
	bool g_deviceLost = false;

	// Set while Present is reporting DXGI_STATUS_OCCLUDED, so IsOccluded knows
	// there is something to probe for.  Clearing it needs DXGI's answer, not a
	// window message: an occluded swap chain becomes visible again without the
	// window itself changing at all (the covering window moves, the taskbar
	// preview closes), so nothing would tell us locally.
	bool g_occluded = false;

	// Which of the two pipelines the context currently has bound.  Every batch
	// needs the same input layout, topology, buffers and shaders as the last
	// batch of its kind, and batches break on every texture change - so binding
	// them per flush meant hundreds of redundant context calls a frame.  Only a
	// switch between the pipelines, or the start of a frame, actually rebinds.
	enum class BoundPipeline { None, Sprite, Debug };
	BoundPipeline g_boundPipeline = BoundPipeline::None;

	// --- Frame state ------------------------------------------------------
	XMFLOAT4X4 g_viewMatrix;
	XMFLOAT4X4 g_projectionMatrix;

	std::vector<SpriteInstance> g_pendingSprites;
	ID3D11ShaderResourceView*   g_pendingTexture = nullptr;   // borrowed, not owned
	std::vector<DebugVertex>    g_pendingDebugVertices;

	// Where the next batch will be written inside the instance ring.
	UINT g_instanceRingOffset = 0;

	// Memoised texture lookup.  Sprites arrive in long runs off the same sheet -
	// that is exactly why flushing on a texture change batches well - and
	// TEXTURE_ID is a variant over ~20 enum types whose std::hash is not free.
	// This turns the common case into one variant comparison.
	std::optional<TEXTURE_ID> g_memoTextureId;
	TEXTURE                   g_memoTexture;

	int g_reportedFailures = 0;

	// Logs graphics errors and alerts the user (first 3 failures).
	void Report(LPCWSTR what, HRESULT hr = S_OK)
	{
		wchar_t message[Constants::Graphics::FATAL_MESSAGE_BUFFER_LENGTH];
		if (hr == S_OK)
			_snwprintf_s(message, ARRAYSIZE(message), _TRUNCATE, L"GraphicsHelper: %s\n", what);
		else
			_snwprintf_s(message, ARRAYSIZE(message), _TRUNCATE, L"GraphicsHelper: %s (hr = 0x%08X)\n", what, (unsigned)hr);

		OutputDebugStringW(message);
		if (++g_reportedFailures <= 3)
			MessageBoxW(nullptr, message, L"NESContra - graphics error", MB_OK | MB_ICONERROR);
	}

	bool Check(HRESULT hr, LPCWSTR what)
	{
		if (FAILED(hr)) { Report(what, hr); return false; }
		return true;
	}

	// Loads precompiled shader bytecode (.cso) or falls back to compiling .hlsl.
	std::wstring PathNextToExecutable(LPCWSTR fileName)
	{
		wchar_t modulePath[MAX_PATH] = {};
		if (GetModuleFileNameW(nullptr, modulePath, MAX_PATH) == 0)
			return fileName;

		wchar_t* lastSlash = wcsrchr(modulePath, L'\\');
		if (lastSlash) *(lastSlash + 1) = L'\0';

		return std::wstring(modulePath) + fileName;
	}

	bool LoadShaderBytecode(LPCWSTR compiledName, LPCWSTR sourceName,
	                        const char* entryPoint, const char* profile,
	                        std::vector<char>& bytecodeOut, ComPtr<ID3DBlob>& blobOut,
	                        bool allowPrecompiled)
	{
		// 1. Pre-compiled object produced by the build.
		if (allowPrecompiled)
		{
			const std::wstring compiledPath = PathNextToExecutable(compiledName);
			HANDLE file = CreateFileW(compiledPath.c_str(), GENERIC_READ, FILE_SHARE_READ,
				nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
			if (file != INVALID_HANDLE_VALUE)
			{
				LARGE_INTEGER size = {};
				if (GetFileSizeEx(file, &size) && size.QuadPart > 0 && size.QuadPart < Constants::Graphics::MAX_SHADER_BYTECODE_BYTES)
				{
					bytecodeOut.resize(static_cast<size_t>(size.QuadPart));
					DWORD read = 0;
					const BOOL ok = ReadFile(file, bytecodeOut.data(),
						static_cast<DWORD>(bytecodeOut.size()), &read, nullptr);
					CloseHandle(file);
					if (ok && read == bytecodeOut.size())
						return true;
				}
				else
				{
					CloseHandle(file);
				}
				bytecodeOut.clear();
			}
		}

		// 2. Runtime fallback: compile the source.
		const std::wstring sourcePath = PathNextToExecutable(sourceName);
		ComPtr<ID3DBlob> errors;
		HRESULT hr = D3DCompileFromFile(sourcePath.c_str(), nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE, entryPoint, profile,
			D3DCOMPILE_ENABLE_STRICTNESS, 0, &blobOut, &errors);

		if (FAILED(hr))
		{
			// Last resort: the working directory (useful when debugging from
			// the project root without a build-time shader copy).
			blobOut.Reset();
			errors.Reset();
			hr = D3DCompileFromFile(sourceName, nullptr,
				D3D_COMPILE_STANDARD_FILE_INCLUDE, entryPoint, profile,
				D3DCOMPILE_ENABLE_STRICTNESS, 0, &blobOut, &errors);
		}

		if (FAILED(hr))
		{
			if (errors) OutputDebugStringA(static_cast<const char*>(errors->GetBufferPointer()));
			wchar_t message[Constants::Graphics::REPORT_MESSAGE_BUFFER_LENGTH];
			_snwprintf_s(message, ARRAYSIZE(message), _TRUNCATE,
				L"could not load shader '%s' (nor compile '%s')",
				compiledName, sourceName);
			Report(message, hr);
			return false;
		}

		return true;
	}

	const void* BytecodePointer(const std::vector<char>& raw, const ComPtr<ID3DBlob>& blob)
	{
		return raw.empty() ? blob->GetBufferPointer() : raw.data();
	}

	SIZE_T BytecodeSize(const std::vector<char>& raw, const ComPtr<ID3DBlob>& blob)
	{
		return raw.empty() ? blob->GetBufferSize() : raw.size();
	}

	// Loads a shader's bytecode and hands it to `create`.
	//
	// If a .cso exists but the device rejects it - stale from an older build,
	// truncated by an interrupted copy, compiled for a different profile - the
	// source is compiled and tried again.  Failing outright would mean a bad file
	// next to the executable bricks the game while the correct .hlsl sits right
	// beside it, which is exactly the case the runtime fallback exists for.
	bool BuildShader(LPCWSTR compiledName, LPCWSTR sourceName,
	                 const char* entryPoint, const char* profile,
	                 std::vector<char>& bytecodeOut, ComPtr<ID3DBlob>& blobOut,
	                 const std::function<HRESULT(const void*, SIZE_T)>& create,
	                 LPCWSTR what)
	{
		for (int attempt = 0; attempt < 2; ++attempt)
		{
			bytecodeOut.clear();
			blobOut.Reset();

			const bool allowPrecompiled = (attempt == 0);
			if (!LoadShaderBytecode(compiledName, sourceName, entryPoint, profile,
			                        bytecodeOut, blobOut, allowPrecompiled))
				return false;

			const HRESULT hr = create(BytecodePointer(bytecodeOut, blobOut),
			                          BytecodeSize(bytecodeOut, blobOut));
			if (SUCCEEDED(hr)) return true;

			// Already compiled from source - a retry would produce the same bytes.
			if (bytecodeOut.empty()) { Check(hr, what); return false; }

			Report(L"a precompiled shader object was rejected; recompiling from source", hr);
		}

		return false;
	}

	// Scales the design resolution uniformly within the client area (letterboxing).
	void UpdateViewport(UINT clientWidth, UINT clientHeight)
	{
		ID3D11DeviceContext* context = GraphicsHelper::context;
		if (!context || clientWidth == 0 || clientHeight == 0) return;

		// Parenthesised so Windows.h's min() macro cannot swallow the call.
		const float scale = (std::min)(
			static_cast<float>(clientWidth)  / GraphicsHelper::DESIGN_WIDTH,
			static_cast<float>(clientHeight) / GraphicsHelper::DESIGN_HEIGHT);

		const float width  = GraphicsHelper::DESIGN_WIDTH  * scale;
		const float height = GraphicsHelper::DESIGN_HEIGHT * scale;

		D3D11_VIEWPORT viewport = {};
		viewport.TopLeftX = (clientWidth  - width)  * 0.5f;
		viewport.TopLeftY = (clientHeight - height) * 0.5f;
		viewport.Width    = width;
		viewport.Height   = height;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		context->RSSetViewports(1, &viewport);

		g_letterboxViewport = viewport;
	}

	bool CreateBackBufferView()
	{
		ComPtr<ID3D11Texture2D> backBuffer;
		HRESULT hr = GraphicsHelper::swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
		if (!Check(hr, L"IDXGISwapChain::GetBuffer failed")) return false;

		hr = GraphicsHelper::device->CreateRenderTargetView(
			backBuffer.Get(), nullptr, &g_renderTargetView);
		return Check(hr, L"CreateRenderTargetView failed");
	}

	// Unpacks a D3DCOLOR (0xAARRGGBB) into a straight-alpha RGBA float4.
	XMFLOAT4 UnpackColour(D3DCOLOR colour)
	{
		return XMFLOAT4(
			((colour >> Constants::Graphics::COLOUR_SHIFT_RED  ) & Constants::Graphics::COLOUR_CHANNEL_MASK) / Constants::Graphics::COLOUR_CHANNEL_MAX,
			((colour >> Constants::Graphics::COLOUR_SHIFT_GREEN) & Constants::Graphics::COLOUR_CHANNEL_MASK) / Constants::Graphics::COLOUR_CHANNEL_MAX,
			((colour >> Constants::Graphics::COLOUR_SHIFT_BLUE ) & Constants::Graphics::COLOUR_CHANNEL_MASK) / Constants::Graphics::COLOUR_CHANNEL_MAX,
			((colour >> Constants::Graphics::COLOUR_SHIFT_ALPHA) & Constants::Graphics::COLOUR_CHANNEL_MASK) / Constants::Graphics::COLOUR_CHANNEL_MAX);
	}

	void FlushSprites();
	void FlushDebugLines();
}

// ===========================================================================
// Initialisation
// ===========================================================================
bool GraphicsHelper::Init(HWND hWnd, UINT clientWidth, UINT clientHeight)
{
	// --- COM, for WIC texture decoding ---
	HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	if (SUCCEEDED(hr))
		// S_OK and S_FALSE BOTH took a reference on the apartment - S_FALSE only
		// means someone had already initialised it the same way, not that the call
		// was a no-op - so both have to be balanced by a CoUninitialize in Cleanup.
		g_comInitialised = true;
	else if (hr == RPC_E_CHANGED_MODE)
		// An STA is already running on this thread.  No reference was taken, so
		// there is nothing for Cleanup to release; WIC works either way.
		g_comInitialised = false;
	else
	{
		Check(hr, L"CoInitializeEx failed");
		return false;
	}

	// --- Device ---
	// SINGLETHREADED: every D3D call in this program is made from the thread that
	// runs WinMain - the game loop directly, and OnResize via WindowProc, which
	// only ever runs inside that loop's DispatchMessage (or synchronously inside
	// ShowWindow, still the same thread).  Declaring that removes the internal
	// locking D3D11 otherwise takes on every context method.
	UINT createFlags = D3D11_CREATE_DEVICE_SINGLETHREADED;
#ifdef _DEBUG
	// Enable D3D11 debug layer in Debug configurations.
	createFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// Feature level 11_0 is a hard floor, not a preference: the shaders are
	// vs_5_0/ps_5_0, which a 10_x device cannot create.  Listing 10_1/10_0 here
	// would only trade a clear "no Direct3D 11 hardware" failure for a confusing
	// one several hundred lines later inside CreateVertexShader.
	const D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};
	D3D_FEATURE_LEVEL obtainedLevel = D3D_FEATURE_LEVEL_11_0;

	hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createFlags,
		featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION,
		&device, &obtainedLevel, &context);

	if (FAILED(hr) && (createFlags & D3D11_CREATE_DEVICE_DEBUG))
	{
		// Fallback if Graphics Tools are not installed.
		OutputDebugStringW(L"GraphicsHelper: D3D11 debug layer unavailable, retrying without it\n");
		createFlags &= ~static_cast<UINT>(D3D11_CREATE_DEVICE_DEBUG);
		hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createFlags,
			featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION,
			&device, &obtainedLevel, &context);
	}
	if (!Check(hr, L"D3D11CreateDevice failed (Direct3D 11 hardware is required)")) return false;

	// Swap chain creation: prefer flip-model, fallback to bitblt
	ComPtr<IDXGIDevice>  dxgiDevice;
	ComPtr<IDXGIAdapter> adapter;
	ComPtr<IDXGIFactory> factory;
	if (SUCCEEDED(device->QueryInterface(IID_PPV_ARGS(&dxgiDevice))) &&
		SUCCEEDED(dxgiDevice->GetAdapter(&adapter)))
	{
		adapter->GetParent(IID_PPV_ARGS(&factory));
	}
	if (!factory) { Report(L"could not reach the DXGI factory"); return false; }

	// Queue at most one frame.  The flip model's default maximum frame latency
	// lets DXGI accept several frames before Present blocks, and because dt is a
	// constant and every timer counts frames, each queued frame is a whole frame
	// between sampling a key and seeing the result.
	ComPtr<IDXGIDevice1> dxgiDevice1;
	if (SUCCEEDED(dxgiDevice.As(&dxgiDevice1)))
		dxgiDevice1->SetMaximumFrameLatency(1);

	ComPtr<IDXGIFactory2> factory2;
	if (SUCCEEDED(factory.As(&factory2)))
	{
		DXGI_SWAP_CHAIN_DESC1 desc = {};
		desc.Width            = clientWidth;
		desc.Height           = clientHeight;
		desc.Format           = DXGI_FORMAT_B8G8R8A8_UNORM;
		desc.SampleDesc.Count = 1;
		desc.BufferUsage      = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		desc.BufferCount      = 2;                                // flip model needs >= 2
		desc.SwapEffect       = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		desc.AlphaMode        = DXGI_ALPHA_MODE_UNSPECIFIED;   // required for HWND swap chains

		ComPtr<IDXGISwapChain1> swapChain1;
		if (SUCCEEDED(factory2->CreateSwapChainForHwnd(device, hWnd, &desc,
			nullptr, nullptr, &swapChain1)))
		{
			swapChain1->QueryInterface(IID_PPV_ARGS(&swapChain));
		}
	}

	if (!swapChain)
	{
		DXGI_SWAP_CHAIN_DESC desc = {};
		desc.BufferCount        = 1;
		desc.BufferDesc.Width   = clientWidth;
		desc.BufferDesc.Height  = clientHeight;
		desc.BufferDesc.Format  = DXGI_FORMAT_B8G8R8A8_UNORM;
		desc.BufferUsage        = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		desc.OutputWindow       = hWnd;
		desc.SampleDesc.Count   = 1;
		desc.Windowed           = TRUE;
		desc.SwapEffect         = DXGI_SWAP_EFFECT_DISCARD;

		hr = factory->CreateSwapChain(device, &desc, &swapChain);
		if (!Check(hr, L"CreateSwapChain failed")) return false;
	}

	// The game handles its own presentation; don't let DXGI hijack Alt+Enter.
	factory->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER);

	if (!CreateBackBufferView()) return false;
	UpdateViewport(clientWidth, clientHeight);

	// --- Sprite shaders ---
	std::vector<char> spriteVSRaw;
	std::vector<char> spritePSRaw;
	ComPtr<ID3DBlob>  spriteVSBlob;
	ComPtr<ID3DBlob>  spritePSBlob;

	// The vertex shader's bytecode is kept: CreateInputLayout needs it below.
	if (!BuildShader(L"SpriteVS.cso", L"SpriteVS.hlsl", "main", "vs_5_0", spriteVSRaw, spriteVSBlob,
		[](const void* bytes, SIZE_T size) {
			return GraphicsHelper::device->CreateVertexShader(bytes, size, nullptr, &g_spriteVS);
		}, L"CreateVertexShader(SpriteVS) failed")) return false;

	if (!BuildShader(L"SpritePS.cso", L"SpritePS.hlsl", "main", "ps_5_0", spritePSRaw, spritePSBlob,
		[](const void* bytes, SIZE_T size) {
			return GraphicsHelper::device->CreatePixelShader(bytes, size, nullptr, &g_spritePS);
		}, L"CreatePixelShader(SpritePS) failed")) return false;

	// Debug line shaders, created only when hitbox drawing is on.
	//
	// The switch is Constants::Graphics::DRAW_HITBOXES - a C++ constant, not a
	// macro - so the `#if DRAW_HITBOXES` this used to be spelled with was an
	// undefined identifier the preprocessor read as 0, and never once compiled
	// its body in.  DrawSprite gates its DrawBox call on the constant instead,
	// so turning the constant on enabled the calls while leaving the pipeline
	// they need uncreated, and DrawBox's `if (!g_debugVertexBuffer) return`
	// silently swallowed every one of them: the feature could not be switched on
	// at all.  `if constexpr` reads the same constant, so both halves now agree.
	std::vector<char> debugVSRaw;
	std::vector<char> debugPSRaw;
	ComPtr<ID3DBlob>  debugVSBlob;
	ComPtr<ID3DBlob>  debugPSBlob;

	if constexpr (Constants::Graphics::DRAW_HITBOXES)
	{
		if (!BuildShader(L"DebugVS.cso", L"DebugVS.hlsl", "main", "vs_5_0", debugVSRaw, debugVSBlob,
			[](const void* bytes, SIZE_T size) {
				return GraphicsHelper::device->CreateVertexShader(bytes, size, nullptr, &g_debugVS);
			}, L"CreateVertexShader(DebugVS) failed")) return false;

		if (!BuildShader(L"DebugPS.cso", L"DebugPS.hlsl", "main", "ps_5_0", debugPSRaw, debugPSBlob,
			[](const void* bytes, SIZE_T size) {
				return GraphicsHelper::device->CreatePixelShader(bytes, size, nullptr, &g_debugPS);
			}, L"CreatePixelShader(DebugPS) failed")) return false;
	}

	// --- Input layouts ---
	// Slot 0 is the shared unit quad; slot 1 steps once per sprite instance.
	const D3D11_INPUT_ELEMENT_DESC spriteLayout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT,       0,  0, D3D11_INPUT_PER_VERTEX_DATA,   0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0,  8, D3D11_INPUT_PER_VERTEX_DATA,   0 },
		{ "WORLD",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1,  0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "WORLD",    1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "WORLD",    2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "WORLD",    3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "TEXCOORD", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 64, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 80, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	};
	hr = device->CreateInputLayout(spriteLayout, ARRAYSIZE(spriteLayout),
		BytecodePointer(spriteVSRaw, spriteVSBlob), BytecodeSize(spriteVSRaw, spriteVSBlob),
		&g_spriteInputLayout);
	if (!Check(hr, L"CreateInputLayout(sprite) failed")) return false;

	if constexpr (Constants::Graphics::DRAW_HITBOXES)
	{
		const D3D11_INPUT_ELEMENT_DESC debugLayout[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		hr = device->CreateInputLayout(debugLayout, ARRAYSIZE(debugLayout),
			BytecodePointer(debugVSRaw, debugVSBlob), BytecodeSize(debugVSRaw, debugVSBlob),
			&g_debugInputLayout);
		if (!Check(hr, L"CreateInputLayout(debug) failed")) return false;
	}

	// --- Static unit quad (LOCAL space, bottom-centre anchored) ---
	constexpr float quadHalfWidth = Constants::Graphics::QUAD_HALF_WIDTH;
	const QuadVertex quadVertices[4] = {
		{ XMFLOAT2(-quadHalfWidth, 1.0f), XMFLOAT2(0.0f, 0.0f) },   // top-left
		{ XMFLOAT2(+quadHalfWidth, 1.0f), XMFLOAT2(1.0f, 0.0f) },   // top-right
		{ XMFLOAT2(-quadHalfWidth, 0.0f), XMFLOAT2(0.0f, 1.0f) },   // bottom-left
		{ XMFLOAT2(+quadHalfWidth, 0.0f), XMFLOAT2(1.0f, 1.0f) },   // bottom-right
	};
	const UINT16 quadIndices[Constants::Graphics::QUAD_INDEX_COUNT] = { 0, 1, 2, 2, 1, 3 };

	D3D11_BUFFER_DESC bufferDesc = {};
	D3D11_SUBRESOURCE_DATA initialData = {};

	bufferDesc.Usage     = D3D11_USAGE_IMMUTABLE;
	bufferDesc.ByteWidth = sizeof(quadVertices);
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	initialData.pSysMem  = quadVertices;
	hr = device->CreateBuffer(&bufferDesc, &initialData, &g_quadVertexBuffer);
	if (!Check(hr, L"CreateBuffer(quad vertices) failed")) return false;

	bufferDesc.ByteWidth = sizeof(quadIndices);
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	initialData.pSysMem  = quadIndices;
	hr = device->CreateBuffer(&bufferDesc, &initialData, &g_quadIndexBuffer);
	if (!Check(hr, L"CreateBuffer(quad indices) failed")) return false;

	// --- Dynamic per-instance / per-frame buffers ---
	bufferDesc = {};
	bufferDesc.Usage          = D3D11_USAGE_DYNAMIC;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	bufferDesc.ByteWidth = sizeof(SpriteInstance) * INSTANCE_RING_CAPACITY;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	hr = device->CreateBuffer(&bufferDesc, nullptr, &g_instanceBuffer);
	if (!Check(hr, L"CreateBuffer(sprite instances) failed")) return false;

	if constexpr (Constants::Graphics::DRAW_HITBOXES)
	{
		bufferDesc.ByteWidth = sizeof(DebugVertex) * MAX_DEBUG_VERTICES;
		hr = device->CreateBuffer(&bufferDesc, nullptr, &g_debugVertexBuffer);
		if (!Check(hr, L"CreateBuffer(debug vertices) failed")) return false;
	}

	bufferDesc.ByteWidth = sizeof(CameraConstants);
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	hr = device->CreateBuffer(&bufferDesc, nullptr, &g_cameraBuffer);
	if (!Check(hr, L"CreateBuffer(camera constants) failed")) return false;

	// Sampler state: POINT filtering, which is both correct for NES-era pixel art
	// and what keeps a sprite from picking up its atlas neighbours - a point
	// sample reads exactly one texel, so it cannot average across a source-rect
	// boundary.  CLAMP is NOT part of that: addressing modes clamp to the
	// TEXTURE's [0,1] range and know nothing about the sub-rect a sprite occupies.
	// So switching Filter to LINEAR would immediately bleed neighbouring atlas
	// cells into every sprite edge, and no address mode would prevent it; that
	// change needs the source rect inset by half a texel in DrawSprite.
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter         = D3D11_FILTER_MIN_MAG_MIP_POINT;
	samplerDesc.AddressU       = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV       = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW       = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MinLOD         = 0.0f;
	samplerDesc.MaxLOD         = D3D11_FLOAT32_MAX;
	hr = device->CreateSamplerState(&samplerDesc, &g_samplerState);
	if (!Check(hr, L"CreateSamplerState failed")) return false;

	// Linear sampler for the post-process / screen-scaling pass
	samplerDesc.Filter         = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
	hr = device->CreateSamplerState(&samplerDesc, &g_linearSamplerState);
	if (!Check(hr, L"CreateSamplerState (linear) failed")) return false;

	// Blend state: Straight alpha blending.
	D3D11_BLEND_DESC blendDesc = {};
	blendDesc.RenderTarget[0].BlendEnable           = TRUE;
	blendDesc.RenderTarget[0].SrcBlend              = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend             = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp               = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha         = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha        = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOpAlpha          = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	hr = device->CreateBlendState(&blendDesc, &g_blendState);
	if (!Check(hr, L"CreateBlendState failed")) return false;

	// Additive blend state for particles.
	D3D11_BLEND_DESC addBlendDesc = {};
	addBlendDesc.RenderTarget[0].BlendEnable           = TRUE;
	addBlendDesc.RenderTarget[0].SrcBlend              = D3D11_BLEND_SRC_ALPHA;
	addBlendDesc.RenderTarget[0].DestBlend             = D3D11_BLEND_ONE;
	addBlendDesc.RenderTarget[0].BlendOp               = D3D11_BLEND_OP_ADD;
	addBlendDesc.RenderTarget[0].SrcBlendAlpha         = D3D11_BLEND_ONE;
	addBlendDesc.RenderTarget[0].DestBlendAlpha        = D3D11_BLEND_ONE;
	addBlendDesc.RenderTarget[0].BlendOpAlpha          = D3D11_BLEND_OP_ADD;
	addBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	hr = device->CreateBlendState(&addBlendDesc, &g_additiveBlendState);
	if (!Check(hr, L"CreateBlendState (additive) failed")) return false;

	// Rasterizer state: Disable culling (mirroring reverses winding order) and MSAA.
	D3D11_RASTERIZER_DESC rasterizerDesc = {};
	rasterizerDesc.FillMode        = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode        = D3D11_CULL_NONE;
	rasterizerDesc.DepthClipEnable = TRUE;
	rasterizerDesc.MultisampleEnable = FALSE;
	hr = device->CreateRasterizerState(&rasterizerDesc, &g_rasterizerState);
	if (!Check(hr, L"CreateRasterizerState failed")) return false;

	// Depth/stencil state: Disabled (render order is submission order).
	D3D11_DEPTH_STENCIL_DESC depthDesc = {};
	depthDesc.DepthEnable   = FALSE;
	depthDesc.StencilEnable = FALSE;
	hr = device->CreateDepthStencilState(&depthDesc, &g_depthStencilState);
	if (!Check(hr, L"CreateDepthStencilState failed")) return false;

	// Projection: Orthographic projection for Y-up design resolution.
	XMStoreFloat4x4(&g_projectionMatrix,
		XMMatrixOrthographicLH(static_cast<float>(DESIGN_WIDTH),
		                       static_cast<float>(DESIGN_HEIGHT), Constants::Graphics::ORTHOGRAPHIC_NEAR_PLANE, Constants::Graphics::ORTHOGRAPHIC_FAR_PLANE));
	XMStoreFloat4x4(&g_viewMatrix, XMMatrixIdentity());

	// (Both pending buffers are reserved once, at the end of Init.)

	// --- WIC, for texture decoding ---
	hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&g_wicFactory));
	if (!Check(hr, L"could not create the WIC imaging factory")) return false;

	// --- Post Process Setup ---
	QueryPerformanceFrequency(&g_timeFrequency);
	QueryPerformanceCounter(&g_timeStart);

	std::vector<char> postVSBytes;
	std::vector<char> postPSBytes;
	ComPtr<ID3DBlob>  postVSBlob;
	ComPtr<ID3DBlob>  postPSBlob;

	if (!BuildShader(L"PostProcessVS.cso", L"PostProcessVS.hlsl", "main", "vs_5_0", postVSBytes, postVSBlob,
		[](const void* bytes, SIZE_T size) {
			return GraphicsHelper::device->CreateVertexShader(bytes, size, nullptr, &g_postProcessVS);
		}, L"CreateVertexShader(PostProcessVS) failed")) return false;

	if (!BuildShader(L"PostProcessPS.cso", L"PostProcessPS.hlsl", "main", "ps_5_0", postPSBytes, postPSBlob,
		[](const void* bytes, SIZE_T size) {
			return GraphicsHelper::device->CreatePixelShader(bytes, size, nullptr, &g_postProcessPS);
		}, L"CreatePixelShader(PostProcessPS) failed")) return false;

	// Create offscreen texture
	D3D11_TEXTURE2D_DESC texDesc = {};
	texDesc.Width            = DESIGN_WIDTH;
	texDesc.Height           = DESIGN_HEIGHT;
	texDesc.MipLevels        = 1;
	texDesc.ArraySize        = 1;
	texDesc.Format           = DXGI_FORMAT_R16G16B16A16_FLOAT;
	texDesc.SampleDesc.Count = 1;
	texDesc.Usage            = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags        = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags   = 0;
	texDesc.MiscFlags        = 0;

	hr = device->CreateTexture2D(&texDesc, nullptr, &g_offscreenTexture);
	if (!Check(hr, L"CreateTexture2D(offscreen) failed")) return false;

	hr = device->CreateRenderTargetView(g_offscreenTexture.Get(), nullptr, &g_offscreenRenderTargetView);
	if (!Check(hr, L"CreateRenderTargetView(offscreen) failed")) return false;

	hr = device->CreateShaderResourceView(g_offscreenTexture.Get(), nullptr, &g_offscreenShaderResourceView);
	if (!Check(hr, L"CreateShaderResourceView(offscreen) failed")) return false;

	// Create post process constant buffer
	D3D11_BUFFER_DESC constBufDesc = {};
	constBufDesc.Usage          = D3D11_USAGE_DYNAMIC;
	constBufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constBufDesc.ByteWidth      = sizeof(PostProcessConstants);
	constBufDesc.BindFlags      = D3D11_BIND_CONSTANT_BUFFER;
	hr = device->CreateBuffer(&constBufDesc, nullptr, &g_postProcessConstantBuffer);
	if (!Check(hr, L"CreateBuffer(post process constants) failed")) return false;

	// Load LUT texture for post-processing from Textures directory
	TEXTURE lutTexture = CreateTexture(L"Resources\\Textures\\vividmemory8-1x.png");
	if (!lutTexture.srv)
		lutTexture = CreateTexture(PathNextToExecutable(L"Resources\\Textures\\vividmemory8-1x.png").c_str());

	if (lutTexture.srv)
	{
		g_lutShaderResourceView.Attach(lutTexture.srv);
	}
	else
	{
		OutputDebugStringW(L"GraphicsHelper: Failed to load LUT texture 'Resources\\Textures\\vividmemory8-1x.png'\n");
	}

	// Create glowing particle dot texture programmatically
	constexpr UINT particleWidth = Constants::Graphics::PARTICLE_TEXTURE_SIZE;
	constexpr UINT particleHeight = Constants::Graphics::PARTICLE_TEXTURE_SIZE;
	constexpr UINT rowPitch = particleWidth * Constants::Graphics::BYTES_PER_PIXEL;
	constexpr UINT imageSize = rowPitch * particleHeight;
	std::vector<UINT8> pixels(imageSize);

	// Derived, not hard-coded: the gradient is a unit disc centred on the
	// texture, so the centre offset is (size - 1) / 2 and the radius is size / 2.
	// Spelling 31.5 and 32.0 out separately silently broke both if size changed.
	constexpr float particleCenterX = (particleWidth  - 1) * 0.5f;
	constexpr float particleCenterY = (particleHeight - 1) * 0.5f;
	constexpr float particleRadiusX = particleWidth  * 0.5f;
	constexpr float particleRadiusY = particleHeight * 0.5f;

	for (UINT y = 0; y < particleHeight; ++y)
	{
		for (UINT x = 0; x < particleWidth; ++x)
		{
			float dx = (static_cast<float>(x) - particleCenterX) / particleRadiusX;
			float dy = (static_cast<float>(y) - particleCenterY) / particleRadiusY;
			float dist = std::hypot(dx, dy);

			float alpha = 0.0f;
			if (dist < 1.0f)
			{
				alpha = std::exp(-Constants::Graphics::PARTICLE_GLOW_FALLOFF_EXPONENT * dist * dist) * (1.0f - dist);
			}

			UINT index = (y * particleWidth + x) * Constants::Graphics::BYTES_PER_PIXEL;
			pixels[index + 0] = Constants::Graphics::COLOUR_CHANNEL_MAX_BYTE; // Blue
			pixels[index + 1] = Constants::Graphics::COLOUR_CHANNEL_MAX_BYTE; // Green
			pixels[index + 2] = Constants::Graphics::COLOUR_CHANNEL_MAX_BYTE; // Red
			pixels[index + 3] = static_cast<UINT8>(alpha * Constants::Graphics::COLOUR_CHANNEL_MAX);
		}
	}

	D3D11_TEXTURE2D_DESC pTexDesc = {};
	pTexDesc.Width            = particleWidth;
	pTexDesc.Height           = particleHeight;
	pTexDesc.MipLevels        = 1;
	pTexDesc.ArraySize        = 1;
	pTexDesc.Format           = DXGI_FORMAT_B8G8R8A8_UNORM;
	pTexDesc.SampleDesc.Count = 1;
	pTexDesc.Usage            = D3D11_USAGE_IMMUTABLE;
	pTexDesc.BindFlags        = D3D11_BIND_SHADER_RESOURCE;

	D3D11_SUBRESOURCE_DATA particleInitialData = {};
	particleInitialData.pSysMem     = pixels.data();
	particleInitialData.SysMemPitch = rowPitch;

	ComPtr<ID3D11Texture2D> particleTexture;
	hr = device->CreateTexture2D(&pTexDesc, &particleInitialData, &particleTexture);
	if (FAILED(hr)) { Check(hr, L"CreateTexture2D (particle) failed"); return false; }

	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc = {};
	viewDesc.Format                    = pTexDesc.Format;
	viewDesc.ViewDimension             = D3D11_SRV_DIMENSION_TEXTURE2D;
	viewDesc.Texture2D.MostDetailedMip = 0;
	viewDesc.Texture2D.MipLevels       = 1;

	hr = device->CreateShaderResourceView(particleTexture.Get(), &viewDesc, &g_particleSRV);
	if (FAILED(hr)) { Check(hr, L"CreateShaderResourceView (particle) failed"); return false; }

	g_pendingSprites.reserve(MAX_SPRITES_PER_BATCH);
	g_pendingDebugVertices.reserve(MAX_DEBUG_VERTICES);

	return true;
}

void GraphicsHelper::Cleanup(void)
{
	for (auto& entry : GraphicsDatabase::textures)
		if (entry.second.srv) { entry.second.srv->Release(); entry.second.srv = nullptr; }
	GraphicsDatabase::textures.clear();
	GraphicsDatabase::sprites.clear();

	// Clear lists to prevent referencing stale sprite IDs.
	GraphicsDatabase::animations.clear();

	g_pendingSprites.clear();
	g_pendingDebugVertices.clear();
	g_pendingTexture     = nullptr;
	g_instanceRingOffset = 0;
	g_boundPipeline      = BoundPipeline::None;   // ClearState below unbinds it all

	// The memo holds a copy of a TEXTURE whose SRV was just released above.
	g_memoTextureId.reset();
	g_memoTexture = TEXTURE();

	g_wicFactory.Reset();
	g_depthStencilState.Reset();
	g_rasterizerState.Reset();
	g_additiveBlendState.Reset();
	g_particleSRV.Reset();
	g_blendState.Reset();
	g_samplerState.Reset();
	g_linearSamplerState.Reset();
	g_cameraBuffer.Reset();
	g_debugVertexBuffer.Reset();
	g_debugInputLayout.Reset();
	g_debugPS.Reset();
	g_debugVS.Reset();
	g_instanceBuffer.Reset();
	g_quadIndexBuffer.Reset();
	g_quadVertexBuffer.Reset();
	g_spriteInputLayout.Reset();
	g_spritePS.Reset();
	g_spriteVS.Reset();
	g_renderTargetView.Reset();

	g_offscreenTexture.Reset();
	g_offscreenRenderTargetView.Reset();
	g_offscreenShaderResourceView.Reset();
	g_lutShaderResourceView.Reset();
	g_postProcessVS.Reset();
	g_postProcessPS.Reset();
	g_postProcessConstantBuffer.Reset();

	if (swapChain) { swapChain->Release(); swapChain = nullptr; }
	if (context)
	{
		// ClearState only unbinds; the destruction that makes possible is deferred
		// until the next command-list submission.  Flushing forces it now, so the
		// device really is the last reference left when it is released - otherwise
		// the debug layer reports the still-live objects at teardown.
		context->ClearState();
		context->Flush();
		context->Release();
		context = nullptr;
	}
	if (device)    { device->Release(); device = nullptr; }

	if (g_comInitialised) { CoUninitialize(); g_comInitialised = false; }

	g_deviceLost = false;
	g_occluded   = false;
}

void GraphicsHelper::OnResize(UINT clientWidth, UINT clientHeight)
{
	if (!swapChain || !device || !context) return;
	if (clientWidth == 0 || clientHeight == 0) return;   // minimised

	context->OMSetRenderTargets(0, nullptr, nullptr);
	g_renderTargetView.Reset();

	// 0 buffer count / 0 format = "keep what the swap chain already has",
	// which is what makes this work for both the flip and bitblt paths.
	HRESULT hr = swapChain->ResizeBuffers(0, clientWidth, clientHeight, DXGI_FORMAT_UNKNOWN, 0);
	if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
	{
		Report(L"the graphics device was lost while resizing the back buffer",
			device->GetDeviceRemovedReason());
		g_deviceLost = true;
		return;
	}

	// Any other failure leaves the render target view released above, so nothing
	// would be drawn from here on; that has to end the frame loop rather than
	// leave a window that quietly stops updating.
	if (!Check(hr, L"ResizeBuffers failed"))   { g_deviceLost = true; return; }
	if (!CreateBackBufferView())               { g_deviceLost = true; return; }
	UpdateViewport(clientWidth, clientHeight);

	// The projection is deliberately NOT rebuilt: it belongs to the design
	// resolution, and UpdateViewport letterboxes it into the new client area.
}

void GraphicsHelper::Clear(float r, float g, float b, float a)
{
	if (!context || !g_offscreenRenderTargetView || !g_renderTargetView) return;

	// Clear offscreen target with the clear color (which clears sprite/game screen)
	const float colour[4] = { r, g, b, a };
	context->ClearRenderTargetView(g_offscreenRenderTargetView.Get(), colour);

	// Clear backbuffer to solid black so letterboxes are always black
	const float black[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	context->ClearRenderTargetView(g_renderTargetView.Get(), black);
}

void GraphicsHelper::Begin(void)
{
	if (!context || !g_offscreenRenderTargetView || !g_renderTargetView) return;

	// Calculate and update running time
	if (g_timeFrequency.QuadPart > 0)
	{
		LARGE_INTEGER current = {};
		QueryPerformanceCounter(&current);
		g_postProcessTime = static_cast<float>(current.QuadPart - g_timeStart.QuadPart) / static_cast<float>(g_timeFrequency.QuadPart);
	}
	g_postProcessFrameCount += 1.0f;

	ID3D11RenderTargetView* views[] = { g_offscreenRenderTargetView.Get() };
	context->OMSetRenderTargets(1, views, nullptr);

	// Set viewport to the off-screen size (640x600)
	D3D11_VIEWPORT offscreenViewport = {};
	offscreenViewport.TopLeftX = 0.0f;
	offscreenViewport.TopLeftY = 0.0f;
	offscreenViewport.Width    = static_cast<float>(DESIGN_WIDTH);
	offscreenViewport.Height   = static_cast<float>(DESIGN_HEIGHT);
	offscreenViewport.MinDepth = 0.0f;
	offscreenViewport.MaxDepth = 1.0f;
	context->RSSetViewports(1, &offscreenViewport);

	// Upload this frame's view and projection matrices once, for every sprite
	// and debug line that follows.
	D3D11_MAPPED_SUBRESOURCE mapped = {};
	if (SUCCEEDED(context->Map(g_cameraBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped)))
	{
		CameraConstants constants;
		constants.view       = g_viewMatrix;
		constants.projection = g_projectionMatrix;
		std::memcpy(mapped.pData, &constants, sizeof(constants));
		context->Unmap(g_cameraBuffer.Get(), 0);
	}

	// Shared state for the whole frame.
	context->RSSetState(g_rasterizerState.Get());
	context->OMSetBlendState(g_blendState.Get(), nullptr, 0xFFFFFFFF);
	context->OMSetDepthStencilState(g_depthStencilState.Get(), 0);

	ID3D11Buffer* cameraBuffers[] = { g_cameraBuffer.Get() };
	context->VSSetConstantBuffers(0, 1, cameraBuffers);

	ID3D11SamplerState* samplers[] = { g_samplerState.Get() };
	context->PSSetSamplers(0, 1, samplers);

	g_pendingSprites.clear();
	g_pendingDebugVertices.clear();
	g_pendingTexture = nullptr;

	// Force one pipeline rebind per frame.  The context would in fact carry its
	// bindings across Present, but the six calls that costs are not worth
	// depending on that - everything else about the frame's shared state is
	// re-established here too.
	g_boundPipeline = BoundPipeline::None;
}

void GraphicsHelper::End(void)
{
	FlushSprites();
	FlushDebugLines();   // debug wireframes draw on top of the scene

	// Run post processing pass
	if (context && g_renderTargetView && g_offscreenShaderResourceView && g_postProcessVS && g_postProcessPS)
	{
		// Set render target to back buffer
		ID3D11RenderTargetView* views[] = { g_renderTargetView.Get() };
		context->OMSetRenderTargets(1, views, nullptr);

		// Set viewport to the letterboxed size (saved in g_letterboxViewport)
		context->RSSetViewports(1, &g_letterboxViewport);

		// Update post-processing constant buffer
		D3D11_MAPPED_SUBRESOURCE mapped = {};
		if (SUCCEEDED(context->Map(g_postProcessConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped)))
		{
			PostProcessConstants constants;
			constants.time             = g_postProcessTime;
			constants.frameCount       = g_postProcessFrameCount;
			constants.canvasSize[0]    = static_cast<float>(DESIGN_WIDTH);
			constants.canvasSize[1]    = static_cast<float>(DESIGN_HEIGHT);
			constants.texelSize[0]     = 1.0f / static_cast<float>(DESIGN_WIDTH);
			constants.texelSize[1]     = 1.0f / static_cast<float>(DESIGN_HEIGHT);
			constants.padding[0]       = 0.0f;
			constants.padding[1]       = 0.0f;
			constants.mousePosition[0] = 0.0f;
			constants.mousePosition[1] = 0.0f;
			constants.mousePosition[2] = 0.0f;
			constants.mousePosition[3] = 0.0f;
			std::memcpy(mapped.pData, &constants, sizeof(constants));
			context->Unmap(g_postProcessConstantBuffer.Get(), 0);
		}

		// Bind shaders and resources
		context->IASetInputLayout(nullptr); // Vertex ID shader has no input layout
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		context->VSSetShader(g_postProcessVS.Get(), nullptr, 0);
		context->PSSetShader(g_postProcessPS.Get(), nullptr, 0);

		ID3D11Buffer* constantBuffers[] = { g_postProcessConstantBuffer.Get() };
		context->PSSetConstantBuffers(0, 1, constantBuffers);

		ID3D11ShaderResourceView* textures[] = {
			g_offscreenShaderResourceView.Get(),
			g_lutShaderResourceView.Get()
		};
		context->PSSetShaderResources(0, 2, textures);

		ID3D11SamplerState* samplers[] = {
			g_linearSamplerState.Get(),
			g_samplerState.Get()
		};
		context->PSSetSamplers(0, 2, samplers);

		// Draw the full-screen triangle/quad
		context->Draw(3, 0);

		// Clean up shader resource slot to avoid warnings about target/resource binding conflicts
		ID3D11ShaderResourceView* nullSRVs[] = { nullptr, nullptr };
		context->PSSetShaderResources(0, 2, nullSRVs);

		// Mark bound pipeline as None so the next frame re-binds correctly
		g_boundPipeline = BoundPipeline::None;
	}
}

GraphicsHelper::PresentResult GraphicsHelper::Present(void)
{
	if (!swapChain || g_deviceLost) return PresentResult::DeviceLost;

	const HRESULT hr = swapChain->Present(1, 0);

	if (hr == S_OK)                 { g_occluded = false; return PresentResult::Presented; }
	if (hr == DXGI_STATUS_OCCLUDED) { g_occluded = true;  return PresentResult::Occluded;  }

	// Everything else is fatal to rendering and must be distinguished from
	// occlusion: a device lost to a TDR or a driver update also stops returning
	// S_OK, and treating that as "just occluded" leaves the game running blind
	// behind a frozen window.
	if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
		Report(L"the graphics device was lost and the game cannot keep rendering",
			device->GetDeviceRemovedReason());
	else
		Report(L"IDXGISwapChain::Present failed", hr);

	g_deviceLost = true;
	return PresentResult::DeviceLost;
}

bool GraphicsHelper::IsOccluded(void)
{
	// A lost device is NOT occluded: answering true would park the caller in a
	// sleep loop instead of letting the next Present report the loss and stop.
	if (!swapChain || g_deviceLost) return false;

	// Nothing to probe for until Present has said so, which is the common case
	// and costs a single comparison.
	if (!g_occluded) return false;

	// DXGI_PRESENT_TEST asks the question without presenting anything: no back
	// buffer is flipped and no vblank is waited on, so this is safe to call on a
	// frame we are about to skip.
	if (swapChain->Present(0, DXGI_PRESENT_TEST) == DXGI_STATUS_OCCLUDED)
		return true;

	g_occluded = false;
	return false;
}

void GraphicsHelper::SetViewMatrix(const D3DMATRIX& view)
{
	// sizeof(g_viewMatrix) == sizeof(D3DMATRIX) == 16 * sizeof(float); using the
	// destination size is robust if either type is ever extended.
	static_assert(sizeof(g_viewMatrix) == sizeof(D3DMATRIX),
	              "XMFLOAT4X4 and D3DMATRIX must be the same size");
	std::memcpy(&g_viewMatrix, &view, sizeof(g_viewMatrix));
}

// ===========================================================================
// Batch flushing
// ===========================================================================
namespace
{
	void FlushSprites()
	{
		ID3D11DeviceContext* context = GraphicsHelper::context;
		if (!context || g_pendingSprites.empty() || !g_pendingTexture)
		{
			g_pendingSprites.clear();
			g_pendingTexture = nullptr;
			return;
		}

		// Cap at the batch limit; DrawSprite already flushes at
		// MAX_SPRITES_PER_BATCH, so this is a defensive belt-and-suspenders guard.
		const UINT count = static_cast<UINT>(
			(std::min)(g_pendingSprites.size(), static_cast<size_t>(MAX_SPRITES_PER_BATCH)));

		// Append into the ring with NO_OVERWRITE, which promises the driver that
		// nothing already in flight is being touched and so costs it nothing.
		// Only a wrap needs DISCARD - that renames the whole buffer, which is
		// precisely what makes it safe to start writing at zero again while the
		// GPU is still reading earlier batches out of the old contents.
		D3D11_MAP mapType = D3D11_MAP_WRITE_NO_OVERWRITE;
		if (g_instanceRingOffset + count > INSTANCE_RING_CAPACITY)
		{
			g_instanceRingOffset = 0;
			mapType              = D3D11_MAP_WRITE_DISCARD;
		}

		D3D11_MAPPED_SUBRESOURCE mapped = {};
		if (SUCCEEDED(context->Map(g_instanceBuffer.Get(), 0, mapType, 0, &mapped)))
		{
			std::memcpy(static_cast<SpriteInstance*>(mapped.pData) + g_instanceRingOffset,
				g_pendingSprites.data(), count * sizeof(SpriteInstance));
			context->Unmap(g_instanceBuffer.Get(), 0);

			// None of this varies between sprite batches.  The instance buffer
			// included: it is bound at offset 0 for every batch, because the
			// ring offset rides on DrawIndexedInstanced's StartInstanceLocation
			// rather than on the vertex-buffer binding.
			if (g_boundPipeline != BoundPipeline::Sprite)
			{
				context->IASetInputLayout(g_spriteInputLayout.Get());
				context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
				context->IASetIndexBuffer(g_quadIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

				ID3D11Buffer* buffers[] = { g_quadVertexBuffer.Get(), g_instanceBuffer.Get() };
				const UINT strides[]    = { sizeof(QuadVertex), sizeof(SpriteInstance) };
				const UINT offsets[]    = { 0, 0 };
				context->IASetVertexBuffers(0, 2, buffers, strides, offsets);

				context->VSSetShader(g_spriteVS.Get(), nullptr, 0);
				context->PSSetShader(g_spritePS.Get(), nullptr, 0);

				g_boundPipeline = BoundPipeline::Sprite;
			}

			// The texture is the one thing a flush exists to change.
			ID3D11ShaderResourceView* resources[] = { g_pendingTexture };
			context->PSSetShaderResources(0, 1, resources);

			if (g_pendingTexture == g_particleSRV.Get())
			{
				context->OMSetBlendState(g_additiveBlendState.Get(), nullptr, 0xFFFFFFFF);
			}
			else
			{
				context->OMSetBlendState(g_blendState.Get(), nullptr, 0xFFFFFFFF);
			}

			// StartInstanceLocation, unlike SV_InstanceID, really does offset the
			// per-instance vertex fetch - so the ring offset needs nothing else.
			context->DrawIndexedInstanced(Constants::Graphics::QUAD_INDEX_COUNT, count, 0, 0, g_instanceRingOffset);

			g_instanceRingOffset += count;
		}

		g_pendingSprites.clear();
		g_pendingTexture = nullptr;
	}

	void FlushDebugLines()
	{
		ID3D11DeviceContext* context = GraphicsHelper::context;

		// Only run when DRAW_HITBOXES is active and debug buffers are initialized.
		if (!context || g_pendingDebugVertices.empty()
			|| !g_debugVertexBuffer || !g_debugVS || !g_debugPS || !g_debugInputLayout)
		{
			g_pendingDebugVertices.clear();
			return;
		}

		D3D11_MAPPED_SUBRESOURCE mapped = {};
		if (SUCCEEDED(context->Map(g_debugVertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped)))
		{
			std::memcpy(mapped.pData, g_pendingDebugVertices.data(),
				g_pendingDebugVertices.size() * sizeof(DebugVertex));
			context->Unmap(g_debugVertexBuffer.Get(), 0);

			// Same deal as FlushSprites: invariant across debug batches, and
			// re-bound only when the sprite pipeline was the last one up.  It
			// stays a check rather than a one-shot because DrawBox flushes when
			// its buffer fills, which can interleave the two mid-frame.
			if (g_boundPipeline != BoundPipeline::Debug)
			{
				context->IASetInputLayout(g_debugInputLayout.Get());
				context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

				ID3D11Buffer* buffers[] = { g_debugVertexBuffer.Get() };
				const UINT strides[]    = { sizeof(DebugVertex) };
				const UINT offsets[]    = { 0 };
				context->IASetVertexBuffers(0, 1, buffers, strides, offsets);

				context->VSSetShader(g_debugVS.Get(), nullptr, 0);
				context->PSSetShader(g_debugPS.Get(), nullptr, 0);

				g_boundPipeline = BoundPipeline::Debug;
			}

			context->Draw(static_cast<UINT>(g_pendingDebugVertices.size()), 0);
		}

		g_pendingDebugVertices.clear();
	}
}

// ===========================================================================
// Texture loading (WIC)
// ===========================================================================
TEXTURE GraphicsHelper::CreateTexture(LPCWSTR textureFilePath)
{
	if (!g_wicFactory || !device)
	{
		Report(L"CreateTexture called before Init succeeded");
		return TEXTURE();
	}

	ComPtr<IWICBitmapDecoder> decoder;
	HRESULT hr = g_wicFactory->CreateDecoderFromFilename(textureFilePath, nullptr,
		GENERIC_READ, WICDecodeMetadataCacheOnLoad, &decoder);
	if (FAILED(hr))
	{
		wchar_t message[Constants::Graphics::REPORT_MESSAGE_BUFFER_LENGTH];
		_snwprintf_s(message, ARRAYSIZE(message), _TRUNCATE,
			L"could not open image '%s'", textureFilePath);
		Report(message, hr);
		return TEXTURE();
	}

	ComPtr<IWICBitmapFrameDecode> frame;
	hr = decoder->GetFrame(0, &frame);
	if (FAILED(hr)) { Report(L"IWICBitmapDecoder::GetFrame failed", hr); return TEXTURE(); }

	UINT width = 0;
	UINT height = 0;
	hr = frame->GetSize(&width, &height);
	if (FAILED(hr) || width == 0 || height == 0)
	{
		Report(L"image has no usable dimensions", hr);
		return TEXTURE();
	}

	// Convert non-BGRA8 textures to 32bpp BGRA.
	ComPtr<IWICBitmapSource> source;
	WICPixelFormatGUID sourceFormat = {};
	hr = frame->GetPixelFormat(&sourceFormat);
	if (FAILED(hr)) { Report(L"IWICBitmapFrameDecode::GetPixelFormat failed", hr); return TEXTURE(); }

	if (sourceFormat == GUID_WICPixelFormat32bppBGRA)
	{
		source = frame;
	}
	else
	{
		ComPtr<IWICFormatConverter> converter;
		hr = g_wicFactory->CreateFormatConverter(&converter);
		if (FAILED(hr)) { Report(L"CreateFormatConverter failed", hr); return TEXTURE(); }

		hr = converter->Initialize(frame.Get(), GUID_WICPixelFormat32bppBGRA,
			WICBitmapDitherTypeNone, nullptr, 0.0f, WICBitmapPaletteTypeMedianCut);
		if (FAILED(hr)) { Report(L"IWICFormatConverter::Initialize failed", hr); return TEXTURE(); }

		source = converter;
	}

	const UINT rowPitch  = width * 4;
	const UINT imageSize = rowPitch * height;
	std::unique_ptr<UINT8[]> pixels(new UINT8[imageSize]);

	hr = source->CopyPixels(nullptr, rowPitch, imageSize, pixels.get());
	if (FAILED(hr)) { Report(L"IWICBitmapSource::CopyPixels failed", hr); return TEXTURE(); }

	// Texture description: standard UNORM format, single mip level.
	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width            = width;
	textureDesc.Height           = height;
	textureDesc.MipLevels        = 1;
	textureDesc.ArraySize        = 1;
	textureDesc.Format           = DXGI_FORMAT_B8G8R8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage            = D3D11_USAGE_IMMUTABLE;
	textureDesc.BindFlags        = D3D11_BIND_SHADER_RESOURCE;

	D3D11_SUBRESOURCE_DATA initialData = {};
	initialData.pSysMem     = pixels.get();
	initialData.SysMemPitch = rowPitch;

	ComPtr<ID3D11Texture2D> texture;
	hr = device->CreateTexture2D(&textureDesc, &initialData, &texture);
	if (FAILED(hr)) { Report(L"CreateTexture2D failed", hr); return TEXTURE(); }

	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc = {};
	viewDesc.Format                    = textureDesc.Format;
	viewDesc.ViewDimension             = D3D11_SRV_DIMENSION_TEXTURE2D;
	viewDesc.Texture2D.MostDetailedMip = 0;
	viewDesc.Texture2D.MipLevels       = 1;

	ID3D11ShaderResourceView* srv = nullptr;   // ownership passes to the TEXTURE
	hr = device->CreateShaderResourceView(texture.Get(), &viewDesc, &srv);
	if (FAILED(hr)) { Report(L"CreateShaderResourceView failed", hr); return TEXTURE(); }

	return TEXTURE(srv, width, height);
}

// ===========================================================================
// Resource helpers
// ===========================================================================
SPRITE GraphicsHelper::CreateSprite(int top, int left, int right, int bottom,
                                    DIRECTION spriteDirection, const TEXTURE_ID& textureId)
{
	return SPRITE(RECT{ left, top, right, bottom }, spriteDirection, textureId);
}

ANIMATION GraphicsHelper::CreateAnimation(TIME defaultTime,
                                          std::vector<std::tuple<SPRITE_ID, TIME>> frames)
{
	for (auto& frame : frames)
		if (std::get<TIME>(frame) == 0)
			std::get<TIME>(frame) = defaultTime;

	return ANIMATION(defaultTime, std::move(frames));
}

void GraphicsHelper::InsertTexture(const TEXTURE_ID& textureId, LPCWSTR textureFilePath)
{
	// Prevent duplicate texture insertions to avoid leaking Shader Resource Views.
	if (GraphicsDatabase::textures.contains(textureId))
	{
		OutputDebugStringW(L"GraphicsHelper: InsertTexture called twice for the same "
		                   L"TEXTURE_ID; keeping the texture already loaded\n");
		return;
	}

	GraphicsDatabase::textures.insert({ textureId, CreateTexture(textureFilePath) });
}

void GraphicsHelper::InsertSprite(const SPRITE_ID& spriteId, int top, int left, int right, int bottom,
                                  DIRECTION spriteDirection, const TEXTURE_ID& textureId)
{
	// Prevent duplicate sprite insertions to avoid leaking allocated RECT memory.
	if (GraphicsDatabase::sprites.contains(spriteId))
	{
		OutputDebugStringW(L"GraphicsHelper: InsertSprite called twice for the same "
		                   L"SPRITE_ID; keeping the sprite already registered\n");
		return;
	}

	GraphicsDatabase::sprites.insert(
		{ spriteId, CreateSprite(top, left, right, bottom, spriteDirection, textureId) });
}

void GraphicsHelper::InsertAnimation(const ANIMATION_ID& animationId, TIME defaultTime,
                                     std::vector<std::tuple<SPRITE_ID, TIME>> frames)
{
	GraphicsDatabase::animations.insert({ animationId, CreateAnimation(defaultTime, std::move(frames)) });
}

// ===========================================================================
// Sprite submission
// ===========================================================================
namespace
{
	const TEXTURE* ResolveTexture(const TEXTURE_ID& textureId)
	{
		if (g_memoTextureId && *g_memoTextureId == textureId)
			return &g_memoTexture;

		const auto found = GraphicsDatabase::textures.find(textureId);
		if (found == GraphicsDatabase::textures.end()) return nullptr;

		// Held by value, so a later rehash of the map cannot invalidate it.
		g_memoTextureId = textureId;
		g_memoTexture   = found->second;
		return &g_memoTexture;
	}
}

void GraphicsHelper::DrawSprite(const SPRITE& sprite, D3DXVECTOR3 position,
                                DIRECTION movingDirection, float angle)
{
	if (!context) return;

	const RECT&       rect            = std::get<RECT>(sprite);
	const DIRECTION   spriteDirection = std::get<DIRECTION>(sprite);
	const TEXTURE_ID& textureId       = std::get<TEXTURE_ID>(sprite);
	if (std::holds_alternative<BULLET_TEXTURE_ID>(textureId))
	{
		return;
	}

	const TEXTURE* found = ResolveTexture(textureId);
	if (!found) return;

	const TEXTURE& texture = *found;
	if (!texture.srv || texture.width == 0 || texture.height == 0) return;

	const float width  = static_cast<float>(rect.right  - rect.left);
	const float height = static_cast<float>(rect.bottom - rect.top);
	if (width <= 0.0f || height <= 0.0f) return;

	// --- LOCAL -> WORLD ---
	// Compose world matrix: scale (negative X = mirror) -> rotate -> translate
	const bool  mirrored = (movingDirection != spriteDirection);
	const float radians  = XMConvertToRadians(angle);

	const XMMATRIX world =
		XMMatrixScaling(mirrored ? -width : width, height, 1.0f) *
		XMMatrixRotationZ(radians) *
		XMMatrixTranslation(position.x, position.y, position.z);

	// Flush batch on texture change or if full.
	if (g_pendingTexture && g_pendingTexture != texture.srv)
		FlushSprites();
	if (g_pendingSprites.size() >= MAX_SPRITES_PER_BATCH)
		FlushSprites();

	g_pendingTexture = texture.srv;

	SpriteInstance instance;
	XMStoreFloat4x4(&instance.world, world);
	constexpr float eps = Constants::Graphics::TEXEL_INSET_EPSILON;
	instance.sourceRect = XMFLOAT4(
		(static_cast<float>(rect.left)   + eps) / texture.width,
		(static_cast<float>(rect.top)    + eps) / texture.height,
		(static_cast<float>(rect.right)  - eps) / texture.width,
		(static_cast<float>(rect.bottom) - eps) / texture.height);
	instance.tint = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	g_pendingSprites.push_back(instance);

	if constexpr (Constants::Graphics::DRAW_HITBOXES)
	{
		// Draw hitbox overlay relative to sprite anchor.
		DrawBox(position.x - width * 0.5f, position.y,
		        position.x + width * 0.5f, position.y + height,
		        angle, 0xFFFF007F, position.x, position.y);
	}
}

// ===========================================================================
// Debug wireframes
// ===========================================================================
void GraphicsHelper::DrawBox(float left, float bottom, float right, float top,
                             float angle, D3DCOLOR colour,
                             float pivotX, float pivotY)
{
	if (!context) return;

	// Ignore if debug rendering is disabled.
	if (!g_debugVertexBuffer) return;

	// Flush debug batch if full.
	if (g_pendingDebugVertices.size() + Constants::Graphics::DEBUG_VERTICES_PER_BOX > MAX_DEBUG_VERTICES)
		FlushDebugLines();

	const float    radians = XMConvertToRadians(angle);
	const float    c = std::cos(radians);
	const float    s = std::sin(radians);
	const XMFLOAT4 rgba = UnpackColour(colour);

	const XMFLOAT2 corners[4] = {
		XMFLOAT2(left,  bottom), XMFLOAT2(right, bottom),
		XMFLOAT2(right, top   ), XMFLOAT2(left,  top   ),
	};

	// Rotate each corner's offset from pivot, then translate back
	for (int i = 0; i < 4; ++i)
	{
		const XMFLOAT2& a = corners[i];
		const XMFLOAT2& b = corners[(i + 1) % 4];

		const float ax = a.x - pivotX;
		const float ay = a.y - pivotY;
		const float bx = b.x - pivotX;
		const float by = b.y - pivotY;

		g_pendingDebugVertices.push_back({ XMFLOAT3(
			pivotX + ax * c - ay * s,
			pivotY + ax * s + ay * c, 0.0f), rgba });
		g_pendingDebugVertices.push_back({ XMFLOAT3(
			pivotX + bx * c - by * s,
			pivotY + bx * s + by * c, 0.0f), rgba });
	}
}

void GraphicsHelper::DrawParticle(D3DXVECTOR3 position, float size, DirectX::XMFLOAT4 tint)
{
	if (!context || !g_particleSRV) return;

	const XMMATRIX world =
		XMMatrixScaling(size, size, 1.0f) *
		XMMatrixTranslation(position.x, position.y, position.z);

	if (g_pendingTexture && g_pendingTexture != g_particleSRV.Get())
		FlushSprites();
	if (g_pendingSprites.size() >= MAX_SPRITES_PER_BATCH)
		FlushSprites();

	g_pendingTexture = g_particleSRV.Get();

	SpriteInstance instance;
	XMStoreFloat4x4(&instance.world, world);
	instance.sourceRect = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	instance.tint = XMFLOAT4(tint.x, tint.y, tint.z, tint.w);

	g_pendingSprites.push_back(instance);
}


