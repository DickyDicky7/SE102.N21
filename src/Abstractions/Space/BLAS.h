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
		BLAS() = default;
		explicit BLAS(Entity* owner);

		void Build(Entity* owner);
		void Refit();

		Entity* GetOwner() const noexcept { return this->_owner; }
		const AABB& GetWorldAABB() const noexcept { return this->_worldAABB; }
		bool IsComposite() const noexcept { return this->_isComposite; }
		size_t GetPrimitiveCount() const noexcept { return this->_primitives.size(); }
		const std::vector<BLASPrimitive>& GetPrimitives() const noexcept { return this->_primitives; }

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
