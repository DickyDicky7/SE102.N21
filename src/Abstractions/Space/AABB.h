#pragma once

#include "Common.h"
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <limits>

class Entity;
class Camera;

namespace Space
{
	struct AABB
	{
		float minX; // Left (L)
		float minY; // Bottom (B)
		float maxX; // Right (R)
		float maxY; // Top (T)

		constexpr AABB() noexcept
			: minX(0.0f), minY(0.0f), maxX(0.0f), maxY(0.0f)
		{
		}

		constexpr AABB(float l, float b, float r, float t) noexcept
			: minX(l), minY(b), maxX(r), maxY(t)
		{
		}

		static constexpr AABB FromMinMax(float minX, float minY, float maxX, float maxY) noexcept
		{
			return AABB(minX, minY, maxX, maxY);
		}

		static constexpr AABB FromCenterSize(float cx, float cy, float w, float h) noexcept
		{
			const float halfW = w * 0.5f;
			return AABB(cx - halfW, cy, cx + halfW, cy + h);
		}

		static constexpr AABB Inverted() noexcept
		{
			return AABB(
				+std::numeric_limits<float>::infinity(),
				+std::numeric_limits<float>::infinity(),
				-std::numeric_limits<float>::infinity(),
				-std::numeric_limits<float>::infinity()
			);
		}

		static AABB FromEntity(const Entity* entity) noexcept;
		static AABB FromCamera(const Camera* camera) noexcept;

		constexpr float GetL() const noexcept { return this->minX; }
		constexpr float GetR() const noexcept { return this->maxX; }
		constexpr float GetB() const noexcept { return this->minY; }
		constexpr float GetT() const noexcept { return this->maxY; }

		constexpr float Width() const noexcept { return (std::max)(0.0f, this->maxX - this->minX); }
		constexpr float Height() const noexcept { return (std::max)(0.0f, this->maxY - this->minY); }
		constexpr float CenterX() const noexcept { return (this->minX + this->maxX) * 0.5f; }
		constexpr float CenterY() const noexcept { return (this->minY + this->maxY) * 0.5f; }
		constexpr float Area() const noexcept { return this->Width() * this->Height(); }
		constexpr float HalfPerimeter() const noexcept { return this->Width() + this->Height(); }

		constexpr bool IsValid() const noexcept
		{
			return this->maxX >= this->minX && this->maxY >= this->minY;
		}

		constexpr bool Contains(const AABB& other) const noexcept
		{
			return other.minX >= this->minX && other.maxX <= this->maxX
				&& other.minY >= this->minY && other.maxY <= this->maxY;
		}

		constexpr bool Contains(float px, float py) const noexcept
		{
			return px >= this->minX && px <= this->maxX && py >= this->minY && py <= this->maxY;
		}

		constexpr bool Intersects(const AABB& other) const noexcept
		{
			return !(other.minX > this->maxX || other.maxX < this->minX
				||   other.minY > this->maxY || other.maxY < this->minY);
		}

		constexpr AABB Union(const AABB& other) const noexcept
		{
			return AABB(
				(std::min)(this->minX, other.minX),
				(std::min)(this->minY, other.minY),
				(std::max)(this->maxX, other.maxX),
				(std::max)(this->maxY, other.maxY)
			);
		}

		constexpr AABB Intersection(const AABB& other) const noexcept
		{
			return AABB(
				(std::max)(this->minX, other.minX),
				(std::max)(this->minY, other.minY),
				(std::min)(this->maxX, other.maxX),
				(std::min)(this->maxY, other.maxY)
			);
		}

		constexpr AABB Swept(float vx, float vy) const noexcept
		{
			return AABB(
				vx < 0.0f ? this->minX + vx : this->minX,
				vy < 0.0f ? this->minY + vy : this->minY,
				vx > 0.0f ? this->maxX + vx : this->maxX,
				vy > 0.0f ? this->maxY + vy : this->maxY
			);
		}

		constexpr AABB Expand(float margin) const noexcept
		{
			return AABB(this->minX - margin, this->minY - margin, this->maxX + margin, this->maxY + margin);
		}

		constexpr AABB Expand(float mx, float my) const noexcept
		{
			return AABB(this->minX - mx, this->minY - my, this->maxX + mx, this->maxY + my);
		}

		// 2D Morton Code for Space-Filling Curve (Z-order curve) Spatial Sorting
		static uint32_t ExpandBits(uint32_t v) noexcept
		{
			v = (v * 0x00010001u) & 0xFF0000FFu;
			v = (v * 0x00000101u) & 0x0F00F00Fu;
			v = (v * 0x00000011u) & 0xC30C30C3u;
			v = (v * 0x00000005u) & 0x49249249u;
			return v;
		}

		static uint32_t Morton2D(uint32_t x, uint32_t y) noexcept
		{
			return (ExpandBits(y) << 1) | ExpandBits(x);
		}

		uint32_t ComputeMortonCode(float worldMinX, float worldMinY, float invWorldW, float invWorldH) const noexcept
		{
			const float normX = (std::clamp)((this->CenterX() - worldMinX) * invWorldW, 0.0f, 1.0f);
			const float normY = (std::clamp)((this->CenterY() - worldMinY) * invWorldH, 0.0f, 1.0f);
			const uint32_t ix = static_cast<uint32_t>(normX * Constants::Physics::MORTON_COORD_MAX);
			const uint32_t iy = static_cast<uint32_t>(normY * Constants::Physics::MORTON_COORD_MAX);
			return Morton2D(ix, iy);
		}
	};
}
