#pragma once

#include "AABB.h"
#include <vector>
#include <cstdint>
#include <span>

class Entity;

namespace Space
{
	struct BVHPrimitive
	{
		Entity* entity = nullptr;
		size_t instanceIdx = 0;
		AABB aabb;
		float centroidX = 0.0f;
		float centroidY = 0.0f;
	};

	struct BVHNode
	{
		AABB aabb;
		uint32_t left = 0;    // If leaf: primitive offset. If internal: left child index
		uint32_t count = 0;   // If leaf: count of primitives (> 0). If internal: 0
		uint32_t right = ~0u; // If internal: right child index. If leaf: ~0u

		constexpr bool IsLeaf() const noexcept { return this->count > 0; }
	};

	// High-Performance Bounding Volume Hierarchy (BVH) with contiguous node layout and zero per-frame allocation
	class BVH
	{
	public:
		BVH() = default;
		~BVH() = default;

		void Clear() noexcept;
		void Build(std::vector<BVHPrimitive> primitives);
		void Refit();

		bool IsEmpty() const noexcept { return this->_nodes.empty(); }
		size_t GetNodeCount() const noexcept { return this->_nodes.size(); }
		size_t GetPrimitiveCount() const noexcept { return this->_primitives.size(); }
		const AABB& GetRootAABB() const noexcept;
		const std::vector<BVHNode>& GetNodes() const noexcept { return this->_nodes; }
		std::vector<BVHPrimitive>& GetPrimitives() noexcept { return this->_primitives; }
		const std::vector<BVHPrimitive>& GetPrimitives() const noexcept { return this->_primitives; }

		template <typename Func>
		void Query(const AABB& queryBox, Func&& callback) const
		{
			if (this->_nodes.empty())
			{
				return;
			}

			// Fixed stack for iterative traversal
			uint32_t stack[Constants::Physics::BVH_TRAVERSAL_STACK_CAPACITY];
			int stackPtr = 0;
			stack[stackPtr++] = 0; // Root node

			while (stackPtr > 0)
			{
				const uint32_t nodeIdx = stack[--stackPtr];
				const auto& node = this->_nodes[nodeIdx];

				if (!node.aabb.Intersects(queryBox))
				{
					continue;
				}

				if (node.IsLeaf())
				{
					for (uint32_t i = 0; i < node.count; ++i)
					{
						const auto& prim = this->_primitives[node.left + i];
						if (prim.aabb.Intersects(queryBox))
						{
							callback(prim.entity, prim.instanceIdx);
						}
					}
				}
				else
				{
					if (node.left != ~0u)  stack[stackPtr++] = node.left;
					if (node.right != ~0u) stack[stackPtr++] = node.right;
				}
			}
		}

	private:
		uint32_t BuildRecursive(uint32_t start, uint32_t end);

		std::vector<BVHNode> _nodes;
		std::vector<BVHPrimitive> _primitives;
	};
}
