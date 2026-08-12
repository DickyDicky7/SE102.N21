#include "ParticleSystem.h"
#include "GraphicsHelper.h"
#include <cmath>
#include <cstdlib>

std::vector<BulletParticle> BulletParticleSystem::particles;

void BulletParticleSystem::AddParticle(float x, float y, float vx, float vy, float size, float life, DirectX::XMFLOAT4 color, bool isLaser, float laserEndX, float laserEndY)
{
	BulletParticle p;
	p.x = x;
	p.y = y;
	p.vx = vx;
	p.vy = vy;
	p.size = size;
	p.maxLife = life;
	p.life = life;
	p.color = color;
	p.isLaser = isLaser;
	p.laserEndX = laserEndX;
	p.laserEndY = laserEndY;
	particles.push_back(p);
}

void BulletParticleSystem::Update()
{
	for (auto it = particles.begin(); it != particles.end(); )
	{
		it->life -= 1.0f / 60.0f;
		if (it->life <= 0.0f)
		{
			it = particles.erase(it);
		}
		else
		{
			it->x += it->vx;
			it->y += it->vy;

			// Apply subtle deceleration (air resistance)
			it->vx *= 0.96f;
			it->vy *= 0.96f;

			++it;
		}
	}
}

void BulletParticleSystem::Render()
{
	for (const auto& p : particles)
	{
		float alpha = p.life / p.maxLife;
		DirectX::XMFLOAT4 col = p.color;
		col.w *= alpha;

		if (p.isLaser)
		{
			float dx = p.laserEndX - p.x;
			float dy = p.laserEndY - p.y;
			float dist = std::sqrt(dx * dx + dy * dy);
			int numSegments = static_cast<int>(dist / 2.0f) + 1;
			for (int i = 0; i <= numSegments; ++i)
			{
				float t = static_cast<float>(i) / static_cast<float>(numSegments);
				float px = p.x + dx * t;
				float py = p.y + dy * t;
				GraphicsHelper::DrawParticle(D3DXVECTOR3(px, py, 0.0f), p.size, col);
			}
		}
		else
		{
			GraphicsHelper::DrawParticle(D3DXVECTOR3(p.x, p.y, 0.0f), p.size, col);
		}
	}
}

void BulletParticleSystem::Clear()
{
	particles.clear();
}
