#pragma once

#include "AABB.h"
#include <vector>
#include <functional>

class Entity;

namespace Space
{
	struct BLASPrimitive
	{
		Entity* entity = nullptr;
		AABB aabb;
	};

	// Bottom-Level Acceleration Structure (BLAS)
	// Encapsulates geometry / sub-primitives for a single entity or composite entity (e.g. multi-joint bosses, bridges, clusters)
	class BLAS
	{
	public:
		BLAS();
		explicit BLAS(Entity* owner);

		void Build(Entity* owner);
		void Refit();

		Entity* GetOwner() const noexcept;
		const AABB& GetWorldAABB() const noexcept;
		bool IsComposite() const noexcept;
		size_t GetPrimitiveCount() const noexcept;
		const std::vector<BLASPrimitive>& GetPrimitives() const noexcept;

		template <typename Func>
		void Query(const AABB& queryBox, Func&& callback) const
		{
			if (!this->_worldAABB.Intersects(queryBox))
			{
				return;
			}

			if (!this->_isComposite)
			{
				if (this->_owner)
				{
					callback(this->_owner);
				}
				return;
			}

			for (const auto& prim : this->_primitives)
			{
				if (prim.aabb.Intersects(queryBox))
				{
					callback(prim.entity);
				}
			}
		}

		template <typename Func>
		void ForEach(Func&& callback) const
		{
			if (!this->_isComposite)
			{
				if (this->_owner)
				{
					callback(this->_owner);
				}
				return;
			}

			for (const auto& prim : this->_primitives)
			{
				callback(prim.entity);
			}
		}

	private:
		Entity* _owner = nullptr;
		AABB _worldAABB;
		std::vector<BLASPrimitive> _primitives;
		bool _isComposite = false;
	};
}
