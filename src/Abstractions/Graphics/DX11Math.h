#pragma once

// DX11Math.h - Compatibility shim providing D3DX9 math types and functions
// on top of DirectXMath. Allows game-logic files to compile without modifications.

#include <Windows.h>       // FLOAT, DWORD, BOOL, RECT, etc.
#include <DirectXMath.h>
#include <cmath>
#include <cstring>

// Constant for PI referenced by game-logic files
#ifndef D3DX_PI
#define D3DX_PI 3.14159265358979323846f
#endif

// D3DCOLOR and color packing macros
typedef DWORD D3DCOLOR;

#ifndef D3DCOLOR_ARGB
#define D3DCOLOR_ARGB(a,r,g,b) (D3DCOLOR)(((DWORD)(a) << 24) | ((DWORD)(r) << 16) | ((DWORD)(g) << 8) | (DWORD)(b))
#endif

#ifndef D3DCOLOR_XRGB
#define D3DCOLOR_XRGB(r,g,b) D3DCOLOR_ARGB(0xFF,(r),(g),(b))
#endif

// D3DXVECTOR2
struct D3DXVECTOR2
{
	FLOAT x;
	FLOAT y;

	D3DXVECTOR2() : x(0.0f), y(0.0f) {}
	D3DXVECTOR2(FLOAT X, FLOAT Y) : x(X), y(Y) {}
	
	// Explicit to prevent implicit scalar-to-vector conversions (e.g., vec == 0)
	explicit D3DXVECTOR2(FLOAT f) : x(f), y(f) {}
	explicit D3DXVECTOR2(INT i) : x((FLOAT)i), y((FLOAT)i) {}
	D3DXVECTOR2(const D3DXVECTOR2& v) : x(v.x), y(v.y) {}

	D3DXVECTOR2& operator=(const D3DXVECTOR2& v) { x = v.x; y = v.y; return *this; }
	D3DXVECTOR2  operator+() const { return *this; }
	D3DXVECTOR2  operator-() const { return D3DXVECTOR2(-x, -y); }
	D3DXVECTOR2  operator+(const D3DXVECTOR2& v) const { return D3DXVECTOR2(x + v.x, y + v.y); }
	D3DXVECTOR2  operator-(const D3DXVECTOR2& v) const { return D3DXVECTOR2(x - v.x, y - v.y); }
	D3DXVECTOR2  operator*(FLOAT s)     const { return D3DXVECTOR2(x * s, y * s); }
	D3DXVECTOR2  operator/(FLOAT s)     const { return D3DXVECTOR2(x / s, y / s); }
	D3DXVECTOR2& operator+=(const D3DXVECTOR2& v) { x += v.x; y += v.y; return *this; }
	D3DXVECTOR2& operator-=(const D3DXVECTOR2& v) { x -= v.x; y -= v.y; return *this; }
	D3DXVECTOR2& operator*=(FLOAT s)           { x *= s; y *= s; return *this; }
	D3DXVECTOR2& operator/=(FLOAT s)           { x /= s; y /= s; return *this; }
	bool operator==(const D3DXVECTOR2& v) const { return x == v.x && y == v.y; }
	bool operator!=(const D3DXVECTOR2& v) const { return !(*this == v); }

	operator FLOAT*()              { return &x; }
	operator const FLOAT*() const  { return &x; }
};

// D3DXVECTOR3
struct D3DXVECTOR3
{
	FLOAT x;
	FLOAT y;
	FLOAT z;

	D3DXVECTOR3() : x(0.0f), y(0.0f), z(0.0f) {}
	D3DXVECTOR3(FLOAT X, FLOAT Y, FLOAT Z) : x(X), y(Y), z(Z) {}
	explicit D3DXVECTOR3(FLOAT f) : x(f), y(f), z(f) {}   // Explicit to prevent implicit conversions
	D3DXVECTOR3(const D3DXVECTOR3& v) : x(v.x), y(v.y), z(v.z) {}
	// Explicit: real D3DX9 had no such constructor, so leaving it implicit would
	// silently let a D3DXVECTOR2 stand in for a D3DXVECTOR3 across every file
	// that includes this shim.
	explicit D3DXVECTOR3(const D3DXVECTOR2& v, FLOAT Z = 0.0f) : x(v.x), y(v.y), z(Z) {}

	D3DXVECTOR3& operator=(const D3DXVECTOR3& v) { x = v.x; y = v.y; z = v.z; return *this; }
	D3DXVECTOR3  operator+() const { return *this; }
	D3DXVECTOR3  operator-() const { return D3DXVECTOR3(-x, -y, -z); }
	D3DXVECTOR3  operator+(const D3DXVECTOR3& v) const { return D3DXVECTOR3(x + v.x, y + v.y, z + v.z); }
	D3DXVECTOR3  operator-(const D3DXVECTOR3& v) const { return D3DXVECTOR3(x - v.x, y - v.y, z - v.z); }
	D3DXVECTOR3  operator*(FLOAT s)     const { return D3DXVECTOR3(x * s, y * s, z * s); }
	D3DXVECTOR3  operator/(FLOAT s)     const { return D3DXVECTOR3(x / s, y / s, z / s); }
	D3DXVECTOR3& operator+=(const D3DXVECTOR3& v) { x += v.x; y += v.y; z += v.z; return *this; }
	D3DXVECTOR3& operator-=(const D3DXVECTOR3& v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
	D3DXVECTOR3& operator*=(FLOAT s)           { x *= s; y *= s; z *= s; return *this; }
	D3DXVECTOR3& operator/=(FLOAT s)           { x /= s; y /= s; z /= s; return *this; }
	bool operator==(const D3DXVECTOR3& v) const { return x == v.x && y == v.y && z == v.z; }
	bool operator!=(const D3DXVECTOR3& v) const { return !(*this == v); }

	operator FLOAT*()              { return &x; }
	operator const FLOAT*() const  { return &x; }
};

// D3DMATRIX / D3DXMATRIX definitions matching D3D9 layout
struct D3DMATRIX
{
	union
	{
		struct
		{
			FLOAT _11, _12, _13, _14;
			FLOAT _21, _22, _23, _24;
			FLOAT _31, _32, _33, _34;
			FLOAT _41, _42, _43, _44;
		};
		FLOAT m[16];
	};

	D3DMATRIX() { std::memset(this, 0, sizeof(D3DMATRIX)); }

	operator FLOAT*()             { return m; }
	operator const FLOAT*() const { return m; }
};

struct D3DXMATRIX : public D3DMATRIX
{
	D3DXMATRIX() { std::memset(this, 0, sizeof(D3DXMATRIX)); }
	explicit D3DXMATRIX(const FLOAT* pArray) { std::memcpy(m, pArray, sizeof(FLOAT) * 16); }

	FLOAT& operator()(UINT row, UINT col)             { return m[row * 4 + col]; }
	FLOAT  operator()(UINT row, UINT col) const       { return m[row * 4 + col]; }

	D3DXMATRIX& operator=(const D3DXMATRIX& o) { std::memcpy(m, o.m, sizeof(FLOAT) * 16); return *this; }
	D3DXMATRIX& operator=(const D3DMATRIX&  o) { std::memcpy(m, o.m, sizeof(FLOAT) * 16); return *this; }
	D3DXMATRIX  operator*(const D3DXMATRIX& o) const;
	D3DXMATRIX& operator*=(const D3DXMATRIX& o);

	operator FLOAT*()             { return m; }
	operator const FLOAT*() const { return m; }
};

// D3DX angle / vector helper functions
inline FLOAT D3DXToRadian(FLOAT fDegrees) { return DirectX::XMConvertToRadians(fDegrees); }
inline FLOAT D3DXToDegree(FLOAT fRadians) { return DirectX::XMConvertToDegrees(fRadians); }

inline FLOAT D3DXVec2Length(const D3DXVECTOR2* v)
{
	return sqrtf(v->x * v->x + v->y * v->y);
}

inline D3DXVECTOR2* D3DXVec2Normalize(D3DXVECTOR2* out, const D3DXVECTOR2* v)
{
	FLOAT len = D3DXVec2Length(v);
	if (len > 0.0f) { out->x = v->x / len; out->y = v->y / len; }
	else            { out->x = 0.0f;       out->y = 0.0f; }
	return out;
}

// D3DXMATRIX arithmetic (using DirectXMath)
inline D3DXMATRIX& D3DXMATRIX::operator*=(const D3DXMATRIX& o)
{
	DirectX::XMMATRIX a = XMLoadFloat4x4(reinterpret_cast<const DirectX::XMFLOAT4X4*>(this));
	DirectX::XMMATRIX b = XMLoadFloat4x4(reinterpret_cast<const DirectX::XMFLOAT4X4*>(&o));
	DirectX::XMMATRIX r = DirectX::XMMatrixMultiply(a, b);
	XMStoreFloat4x4(reinterpret_cast<DirectX::XMFLOAT4X4*>(this), r);
	return *this;
}

inline D3DXMATRIX D3DXMATRIX::operator*(const D3DXMATRIX& o) const
{
	DirectX::XMMATRIX a = XMLoadFloat4x4(reinterpret_cast<const DirectX::XMFLOAT4X4*>(this));
	DirectX::XMMATRIX b = XMLoadFloat4x4(reinterpret_cast<const DirectX::XMFLOAT4X4*>(&o));
	DirectX::XMMATRIX r = DirectX::XMMatrixMultiply(a, b);
	D3DXMATRIX result;
	XMStoreFloat4x4(reinterpret_cast<DirectX::XMFLOAT4X4*>(&result), r);
	return result;
}

// D3D9-style matrix construction functions backed by DirectXMath
inline VOID D3DXMatrixLookAtLH(D3DXMATRIX* pOut, const D3DXVECTOR3* pEye, const D3DXVECTOR3* pAt, const D3DXVECTOR3* pUp)
{
	using namespace DirectX;
	XMVECTOR eye = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(pEye));
	XMVECTOR at  = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(pAt));
	XMVECTOR up  = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(pUp));
	XMStoreFloat4x4(
		reinterpret_cast<XMFLOAT4X4*>(pOut),
		XMMatrixLookAtLH(eye, at, up)
	);
}

inline VOID D3DXMatrixScaling(D3DXMATRIX* pOut, FLOAT sx, FLOAT sy, FLOAT sz)
{
	DirectX::XMStoreFloat4x4(
		reinterpret_cast<DirectX::XMFLOAT4X4*>(pOut),
		DirectX::XMMatrixScaling(sx, sy, sz)
	);
}

