#pragma once

#include <vector>
#include <d3d11.h>
#include <DirectXMath.h>
#include "Common.h"

struct BulletParticle
{
	float x, y;
	float vx, vy;
	float size;
	float maxLife;
	float life;
	DirectX::XMFLOAT4 color;
	bool isLaser;
	float laserEndX, laserEndY;
};

class BulletParticleSystem
{
public:
	static void AddParticle(float x, float y, float vx, float vy, float size, float life, DirectX::XMFLOAT4 color, bool isLaser = false, float laserEndX = 0.0f, float laserEndY = 0.0f);
	static void Update();
	static void Render();
	static void Clear();

private:
	static std::vector<BulletParticle> particles;
};
