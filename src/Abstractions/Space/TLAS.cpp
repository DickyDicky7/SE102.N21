#include "TLAS.h"
#include "Entity.h"

namespace Space
{
	TLAS::TLAS()
		: _bounds(AABB::Inverted()), _isDirty(true)
	{
	}

	TLAS::~TLAS()
	{
		this->Clear();
	}

	void TLAS::Clear() noexcept
	{
		this->_instances.clear();
		this->_entityToIndex.clear();
		this->_bvh.Clear();
		this->_bounds = AABB::Inverted();
		this->_isDirty = true;
	}

	void TLAS::Clean()
	{
		for (auto& inst : this->_instances)
		{
			if (inst.entity)
			{
				Destroy(inst.entity);
			}
		}
		this->Clear();
	}

	void TLAS::Insert(Entity* entity)
	{
		if (!entity)
		{
			return;
		}

		auto it = this->_entityToIndex.find(entity);
		if (it != this->_entityToIndex.end())
		{
			auto& inst = this->_instances[it->second];
			inst.active = true;
			inst.blas.Build(entity);
			inst.worldAABB = inst.blas.GetWorldAABB();
			this->_isDirty = true;
			return;
		}

		TLASInstance inst;
		inst.entity = entity;
		inst.blas.Build(entity);
		inst.worldAABB = inst.blas.GetWorldAABB();
		inst.active = true;

		const size_t index = this->_instances.size();
		this->_instances.push_back(std::move(inst));
		this->_entityToIndex[entity] = index;
		this->_isDirty = true;
	}

	void TLAS::Remove(Entity* entity)
	{
		if (!entity)
		{
			return;
		}

		auto it = this->_entityToIndex.find(entity);
		if (it != this->_entityToIndex.end())
		{
			const size_t idx = it->second;
			if (idx < this->_instances.size())
			{
				this->_instances[idx].active = false;
				this->_instances[idx].entity = nullptr;
			}
			this->_entityToIndex.erase(it);
			this->_isDirty = true;
		}
	}

	void TLAS::Build()
	{
		// Compact instances to remove inactive slots
		std::vector<TLASInstance> compacted;
		compacted.reserve(this->_instances.size());
		this->_entityToIndex.clear();

		std::vector<BVHPrimitive> primitives;
		primitives.reserve(this->_instances.size());

		for (auto& inst : this->_instances)
		{
			if (inst.active && inst.entity)
			{
				inst.blas.Refit();
				inst.worldAABB = inst.blas.GetWorldAABB();

				const size_t newIdx = compacted.size();
				this->_entityToIndex[inst.entity] = newIdx;

				BVHPrimitive prim;
				prim.entity = inst.entity;
				prim.instanceIdx = newIdx;
				prim.aabb = inst.worldAABB;
				prim.centroidX = inst.worldAABB.CenterX();
				prim.centroidY = inst.worldAABB.CenterY();
				primitives.push_back(prim);

				compacted.push_back(std::move(inst));
			}
		}

		this->_instances = std::move(compacted);
		this->_bvh.Build(std::move(primitives));
		this->_bounds = this->_bvh.GetRootAABB();
		this->_isDirty = false;
	}

	void TLAS::Update()
	{
		if (this->_isDirty)
		{
			this->Build();
			return;
		}

		// Refit all instance BLASes and update primitives
		for (size_t i = 0; i < this->_instances.size(); ++i)
		{
			auto& inst = this->_instances[i];
			if (inst.active && inst.entity)
			{
				inst.blas.Refit();
				inst.worldAABB = inst.blas.GetWorldAABB();
			}
		}

		auto& bvhPrimitives = this->_bvh.GetPrimitives();
		for (auto& prim : bvhPrimitives)
		{
			if (prim.instanceIdx < this->_instances.size())
			{
				const auto& inst = this->_instances[prim.instanceIdx];
				prim.aabb = inst.worldAABB;
				prim.centroidX = inst.worldAABB.CenterX();
				prim.centroidY = inst.worldAABB.CenterY();
			}
		}

		this->_bvh.Refit();
		this->_bounds = this->_bvh.GetRootAABB();
	}

	size_t TLAS::GetEntityCount() const noexcept
	{
		size_t count = 0;
		for (const auto& inst : this->_instances)
		{
			if (inst.active && inst.entity)
			{
				++count;
			}
		}
		return count;
	}

	const AABB& TLAS::GetBounds() const noexcept
	{
		return this->_bounds;
	}

	void TLAS::FindCollisionsFor(Entity* collidable, const std::function<void(Entity*)>& callback) const
	{
		if (!collidable)
		{
			return;
		}

		const AABB selfBox = AABB::FromEntity(collidable);
		const AABB sweptBox = selfBox.Swept(collidable->GetVX(), collidable->GetVY()).Expand(Constants::Physics::CANDIDATE_EXPAND_EPSILON);

		this->QueryAABB(sweptBox, [&](Entity* candidate) {
			if (candidate && candidate != collidable)
			{
				callback(candidate);
			}
		});
	}
}
