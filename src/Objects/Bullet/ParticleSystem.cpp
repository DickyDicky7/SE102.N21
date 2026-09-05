#include "ParticleSystem.h"
#include "GraphicsHelper.h"
#include <cmath>
#include <cstdlib>

std::vector<BulletParticle> BulletParticleSystem::_particles;

void BulletParticleSystem::AddParticle(float x, float y, float vx, float vy, float size, float life, DirectX::XMFLOAT4 color, bool isLaser, float laserEndX, float laserEndY)
{
	if (_particles.capacity() == 0)
	{
		_particles.reserve(Constants::Graphics::PARTICLE_SYSTEM_RESERVE_CAPACITY);
	}
	if (_particles.size() >= Constants::Graphics::PARTICLE_SYSTEM_MAX_CAPACITY)
	{
		return;
	}

	_particles.push_back(BulletParticle{
		.x = x,
		.y = y,
		.vx = vx,
		.vy = vy,
		.size = size,
		.maxLife = life,
		.life = life,
		.color = color,
		.isLaser = isLaser,
		.laserEndX = laserEndX,
		.laserEndY = laserEndY
	});
}

void BulletParticleSystem::Update()
{
	for (auto& p : _particles)
	{
		p.life -= Constants::Graphics::FRAME_DELTA_TIME;
		p.x += p.vx;
		p.y += p.vy;

		// Apply subtle deceleration (air resistance)
		p.vx *= Constants::Graphics::PARTICLE_AIR_RESISTANCE;
		p.vy *= Constants::Graphics::PARTICLE_AIR_RESISTANCE;
	}

	std::erase_if(_particles, [](const BulletParticle& p) {
		return p.life <= 0.0f;
	});
}

void BulletParticleSystem::Render()
{
	for (const auto& p : _particles)
	{
		float alpha = (p.maxLife > 0.0f) ? (p.life / p.maxLife) : 1.0f;
		DirectX::XMFLOAT4 col = p.color;
		col.w *= alpha;

		if (p.isLaser)
		{
			float dx = p.laserEndX - p.x;
			float dy = p.laserEndY - p.y;
			float dist = std::sqrt(dx * dx + dy * dy);
			int numSegments = static_cast<int>(dist / Constants::Graphics::PARTICLE_LASER_SEGMENT_STEP) + 1;
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
	_particles.clear();
}
