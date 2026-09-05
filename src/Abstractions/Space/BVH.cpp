#include "BVH.h"
#include <algorithm>
#include <limits>

namespace Space
{
	static const AABB s_emptyAABB = AABB::Inverted();

	const AABB& BVH::GetRootAABB() const noexcept
	{
		return this->_nodes.empty() ? s_emptyAABB : this->_nodes[0].aabb;
	}

	void BVH::Clear() noexcept
	{
		this->_nodes.clear();
		this->_primitives.clear();
	}

	void BVH::Build(std::vector<BVHPrimitive> primitives)
	{
		this->Clear();
		this->_primitives = std::move(primitives);

		if (this->_primitives.empty())
		{
			return;
		}

		for (auto& prim : this->_primitives)
		{
			prim.centroidX = prim.aabb.CenterX();
			prim.centroidY = prim.aabb.CenterY();
		}

		this->_nodes.reserve(this->_primitives.size() * 2);
		this->BuildRecursive(0, static_cast<uint32_t>(this->_primitives.size()));
	}

	uint32_t BVH::BuildRecursive(uint32_t start, uint32_t end)
	{
		const uint32_t count = end - start;
		const uint32_t nodeIdx = static_cast<uint32_t>(this->_nodes.size());
		this->_nodes.emplace_back();

		// Compute bounding box for primitives
		AABB bbox = AABB::Inverted();
		float minCentroidX = +std::numeric_limits<float>::infinity();
		float maxCentroidX = -std::numeric_limits<float>::infinity();
		float minCentroidY = +std::numeric_limits<float>::infinity();
		float maxCentroidY = -std::numeric_limits<float>::infinity();

		for (uint32_t i = start; i < end; ++i)
		{
			const auto& prim = this->_primitives[i];
			bbox = bbox.Union(prim.aabb);
			minCentroidX = (std::min)(minCentroidX, prim.centroidX);
			maxCentroidX = (std::max)(maxCentroidX, prim.centroidX);
			minCentroidY = (std::min)(minCentroidY, prim.centroidY);
			maxCentroidY = (std::max)(maxCentroidY, prim.centroidY);
		}

		this->_nodes[nodeIdx].aabb = bbox;

		// Leaf node condition
		if (count <= Constants::Physics::BVH_MAX_LEAF_PRIMITIVES)
		{
			this->_nodes[nodeIdx].left = start;
			this->_nodes[nodeIdx].count = count;
			this->_nodes[nodeIdx].right = ~0u;
			return nodeIdx;
		}

		const float extentX = maxCentroidX - minCentroidX;
		const float extentY = maxCentroidY - minCentroidY;

		if (extentX <= 0.0f && extentY <= 0.0f)
		{
			this->_nodes[nodeIdx].left = start;
			this->_nodes[nodeIdx].count = count;
			this->_nodes[nodeIdx].right = ~0u;
			return nodeIdx;
		}

		// Partition along axis with larger centroid extent
		const uint32_t mid = start + count / 2;
		if (extentX >= extentY)
		{
			std::nth_element(
				this->_primitives.begin() + start,
				this->_primitives.begin() + mid,
				this->_primitives.begin() + end,
				[](const BVHPrimitive& a, const BVHPrimitive& b) {
					return a.centroidX < b.centroidX;
				}
			);
		}
		else
		{
			std::nth_element(
				this->_primitives.begin() + start,
				this->_primitives.begin() + mid,
				this->_primitives.begin() + end,
				[](const BVHPrimitive& a, const BVHPrimitive& b) {
					return a.centroidY < b.centroidY;
				}
			);
		}

		const uint32_t leftChild = this->BuildRecursive(start, mid);
		const uint32_t rightChild = this->BuildRecursive(mid, end);

		this->_nodes[nodeIdx].left = leftChild;
		this->_nodes[nodeIdx].right = rightChild;
		this->_nodes[nodeIdx].count = 0;
		this->_nodes[nodeIdx].aabb = this->_nodes[leftChild].aabb.Union(this->_nodes[rightChild].aabb);

		return nodeIdx;
	}

	void BVH::Refit()
	{
		if (this->_nodes.empty() || this->_primitives.empty())
		{
			return;
		}

		// Bottom-up traversal in reverse order of node indices
		for (int i = static_cast<int>(this->_nodes.size()) - 1; i >= 0; --i)
		{
			auto& node = this->_nodes[i];
			if (node.IsLeaf())
			{
				AABB box = AABB::Inverted();
				for (uint32_t p = 0; p < node.count; ++p)
				{
					box = box.Union(this->_primitives[node.left + p].aabb);
				}
				node.aabb = box;
			}
			else
			{
				AABB box = AABB::Inverted();
				if (node.left != ~0u)  box = box.Union(this->_nodes[node.left].aabb);
				if (node.right != ~0u) box = box.Union(this->_nodes[node.right].aabb);
				node.aabb = box;
			}
		}
	}
}
