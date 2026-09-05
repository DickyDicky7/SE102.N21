#pragma once

#include "AABB.h"
#include "BLAS.h"
#include "BVH.h"
#include <vector>
#include <unordered_map>
#include <functional>

class Entity;
class Camera;

namespace Space
{
	struct TLASInstance
	{
		Entity* entity = nullptr;
		BLAS blas;
		AABB worldAABB;
		bool active = true;
	};

	// Top-Level Acceleration Structure (TLAS)
	// Manages world/scene instances, aggregates BLAS sub-structures, and drives broadphase collision and frustum culling
	class TLAS
	{
	public:
		TLAS();
		~TLAS();

		void Clear() noexcept;
		void Clean(); // Deletes all contained entities and clears structure

		void Insert(Entity* entity);
		void Remove(Entity* entity);

		void Build();
		void Update();

		bool IsEmpty() const noexcept { return this->_instances.empty(); }
		size_t GetEntityCount() const noexcept;
		const AABB& GetBounds() const noexcept;
		const std::vector<TLASInstance>& GetInstances() const noexcept { return this->_instances; }

		// Range / Box Query against TLAS top-level instances
		template <typename Func>
		void QueryAABB(const AABB& queryBox, Func&& callback) const
		{
			if (this->_instances.empty())
			{
				return;
			}

			if (this->_isDirty)
			{
				const_cast<TLAS*>(this)->Build();
			}

			this->_bvh.Query(queryBox, [&](Entity* /*entity*/, size_t instanceIdx) {
				if (instanceIdx < this->_instances.size())
				{
					const auto& inst = this->_instances[instanceIdx];
					if (inst.active && inst.entity)
					{
						callback(inst.entity);
					}
				}
			});
		}

		// Camera Frustum / Viewport Culling Query
		template <typename Func>
		void QueryFrustum(const Camera* camera, Func&& callback) const
		{
			const AABB camBox = AABB::FromCamera(camera);
			this->QueryAABB(camBox, std::forward<Func>(callback));
		}

		// Swept-AABB Broadphase Query for moving entities
		template <typename Func>
		void QuerySwept(const AABB& sweptBox, Func&& callback) const
		{
			this->QueryAABB(sweptBox, std::forward<Func>(callback));
		}

		void FindCollisionsFor(Entity* collidable, const std::function<void(Entity*)>& callback) const;

	private:
		std::vector<TLASInstance> _instances;
		std::unordered_map<Entity*, size_t> _entityToIndex;
		BVH _bvh;
		AABB _bounds;
		bool _isDirty = true;
	};
}
