#pragma once

// DX11Math.h - Compatibility shim providing D3DX9 math types and functions
// on top of DirectXMath. Allows game-logic files to compile without modifications.

#include "Constants.h"
#include <Windows.h>       // FLOAT, DWORD, BOOL, RECT, etc.
#include <DirectXMath.h>
#include <cmath>
#include <numbers>
#include <cstring>

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
	float x;
	float y;

	D3DXVECTOR2() : x(0.0f), y(0.0f) {}
	D3DXVECTOR2(float inX, float inY) : x(inX), y(inY) {}

	// Explicit to prevent implicit scalar-to-vector conversions (e.g., vec == 0)
	explicit D3DXVECTOR2(float f) : x(f), y(f) {}
	explicit D3DXVECTOR2(int i) : x(static_cast<float>(i)), y(static_cast<float>(i)) {}
	D3DXVECTOR2(const D3DXVECTOR2& v) : x(v.x), y(v.y) {}

	D3DXVECTOR2& operator=(const D3DXVECTOR2& v) { this->x = v.x; this->y = v.y; return *this; }
	D3DXVECTOR2  operator+() const { return *this; }
	D3DXVECTOR2  operator-() const { return D3DXVECTOR2(-this->x, -this->y); }
	D3DXVECTOR2  operator+(const D3DXVECTOR2& v) const { return D3DXVECTOR2(this->x + v.x, this->y + v.y); }
	D3DXVECTOR2  operator-(const D3DXVECTOR2& v) const { return D3DXVECTOR2(this->x - v.x, this->y - v.y); }
	D3DXVECTOR2  operator*(float s)     const { return D3DXVECTOR2(this->x * s, this->y * s); }
	D3DXVECTOR2  operator/(float s)     const { return D3DXVECTOR2(this->x / s, this->y / s); }
	D3DXVECTOR2& operator+=(const D3DXVECTOR2& v) { this->x += v.x; this->y += v.y; return *this; }
	D3DXVECTOR2& operator-=(const D3DXVECTOR2& v) { this->x -= v.x; this->y -= v.y; return *this; }
	D3DXVECTOR2& operator*=(float s)           { this->x *= s; this->y *= s; return *this; }
	D3DXVECTOR2& operator/=(float s)           { this->x /= s; this->y /= s; return *this; }
	bool operator==(const D3DXVECTOR2& v) const { return this->x == v.x && this->y == v.y; }
	bool operator!=(const D3DXVECTOR2& v) const { return !(*this == v); }

	operator float*()              { return &this->x; }
	operator const float*() const  { return &this->x; }
};

// D3DXVECTOR3
struct D3DXVECTOR3
{
	float x;
	float y;
	float z;

	D3DXVECTOR3() : x(0.0f), y(0.0f), z(0.0f) {}
	D3DXVECTOR3(float inX, float inY, float inZ) : x(inX), y(inY), z(inZ) {}
	explicit D3DXVECTOR3(float f) : x(f), y(f), z(f) {}   // Explicit to prevent implicit conversions
	D3DXVECTOR3(const D3DXVECTOR3& v) : x(v.x), y(v.y), z(v.z) {}
	// Explicit: real D3DX9 had no such constructor, so leaving it implicit would
	// silently let a D3DXVECTOR2 stand in for a D3DXVECTOR3 across every file
	// that includes this shim.
	explicit D3DXVECTOR3(const D3DXVECTOR2& v, float inZ = 0.0f) : x(v.x), y(v.y), z(inZ) {}

	D3DXVECTOR3& operator=(const D3DXVECTOR3& v) { this->x = v.x; this->y = v.y; this->z = v.z; return *this; }
	D3DXVECTOR3  operator+() const { return *this; }
	D3DXVECTOR3  operator-() const { return D3DXVECTOR3(-this->x, -this->y, -this->z); }
	D3DXVECTOR3  operator+(const D3DXVECTOR3& v) const { return D3DXVECTOR3(this->x + v.x, this->y + v.y, this->z + v.z); }
	D3DXVECTOR3  operator-(const D3DXVECTOR3& v) const { return D3DXVECTOR3(this->x - v.x, this->y - v.y, this->z - v.z); }
	D3DXVECTOR3  operator*(float s)     const { return D3DXVECTOR3(this->x * s, this->y * s, this->z * s); }
	D3DXVECTOR3  operator/(float s)     const { return D3DXVECTOR3(this->x / s, this->y / s, this->z / s); }
	D3DXVECTOR3& operator+=(const D3DXVECTOR3& v) { this->x += v.x; this->y += v.y; this->z += v.z; return *this; }
	D3DXVECTOR3& operator-=(const D3DXVECTOR3& v) { this->x -= v.x; this->y -= v.y; this->z -= v.z; return *this; }
	D3DXVECTOR3& operator*=(float s)           { this->x *= s; this->y *= s; this->z *= s; return *this; }
	D3DXVECTOR3& operator/=(float s)           { this->x /= s; this->y /= s; this->z /= s; return *this; }
	bool operator==(const D3DXVECTOR3& v) const { return this->x == v.x && this->y == v.y && this->z == v.z; }
	bool operator!=(const D3DXVECTOR3& v) const { return !(*this == v); }

	operator float*()              { return &this->x; }
	operator const float*() const  { return &this->x; }
};

// D3DMATRIX / D3DXMATRIX definitions matching D3D9 layout
struct D3DMATRIX
{
	union
	{
		struct
		{
			float _11, _12, _13, _14;
			float _21, _22, _23, _24;
			float _31, _32, _33, _34;
			float _41, _42, _43, _44;
		};
		float m[Constants::Graphics::MATRIX_ELEMENT_COUNT];
	};

	D3DMATRIX() { std::memset(this, 0, sizeof(D3DMATRIX)); }

	operator float*()             { return this->m; }
	operator const float*() const { return this->m; }
};

struct D3DXMATRIX : public D3DMATRIX
{
	D3DXMATRIX() { std::memset(this, 0, sizeof(D3DXMATRIX)); }
	explicit D3DXMATRIX(const float* pArray) { std::memcpy(this->m, pArray, sizeof(float) * Constants::Graphics::MATRIX_ELEMENT_COUNT); }

	float& operator()(UINT row, UINT col)             { return this->m[row * 4 + col]; }
	float  operator()(UINT row, UINT col) const       { return this->m[row * 4 + col]; }

	D3DXMATRIX& operator=(const D3DXMATRIX& o) { std::memcpy(this->m, o.m, sizeof(float) * Constants::Graphics::MATRIX_ELEMENT_COUNT); return *this; }
	D3DXMATRIX& operator=(const D3DMATRIX&  o) { std::memcpy(this->m, o.m, sizeof(float) * Constants::Graphics::MATRIX_ELEMENT_COUNT); return *this; }
	D3DXMATRIX  operator*(const D3DXMATRIX& o) const;
	D3DXMATRIX& operator*=(const D3DXMATRIX& o);

	operator float*()             { return this->m; }
	operator const float*() const { return this->m; }
};

// D3DX angle / vector helper functions
inline float D3DXToRadian(float fDegrees) { return fDegrees * (std::numbers::pi_v<float> / Constants::Physics::HALF_CIRCLE_DEGREES); }
inline float D3DXToDegree(float fRadians) { return fRadians * (Constants::Physics::HALF_CIRCLE_DEGREES / std::numbers::pi_v<float>); }

inline float D3DXVec2Length(const D3DXVECTOR2* v)
{
	return std::sqrt(v->x * v->x + v->y * v->y);
}

inline D3DXVECTOR2* D3DXVec2Normalize(D3DXVECTOR2* out, const D3DXVECTOR2* v)
{
	float len = D3DXVec2Length(v);
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
inline void D3DXMatrixLookAtLH(D3DXMATRIX* pOut, const D3DXVECTOR3* pEye, const D3DXVECTOR3* pAt, const D3DXVECTOR3* pUp)
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

inline void D3DXMatrixScaling(D3DXMATRIX* pOut, float sx, float sy, float sz)
{
	DirectX::XMStoreFloat4x4(
		reinterpret_cast<DirectX::XMFLOAT4X4*>(pOut),
		DirectX::XMMatrixScaling(sx, sy, sz)
	);
}

static_assert(sizeof(D3DXVECTOR2) == sizeof(DirectX::XMFLOAT2), "D3DXVECTOR2 size must match XMFLOAT2");
static_assert(sizeof(D3DXVECTOR3) == sizeof(DirectX::XMFLOAT3), "D3DXVECTOR3 size must match XMFLOAT3");
static_assert(sizeof(D3DMATRIX) == sizeof(DirectX::XMFLOAT4X4), "D3DMATRIX size must match XMFLOAT4X4");
static_assert(sizeof(D3DXMATRIX) == sizeof(DirectX::XMFLOAT4X4), "D3DXMATRIX size must match XMFLOAT4X4");

