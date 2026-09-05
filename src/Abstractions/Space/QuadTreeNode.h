#pragma once

#include "Common.h"
#include "Entity.h"
#include "Camera.h"
#include "AABB.h"
#include "BLAS.h"
#include "BVH.h"
#include "TLAS.h"
#include <memory>
#include <vector>
#include <unordered_map>

struct QuadTreeNode;

struct BoundingBox
{
	float B;
	float T;
	float L;
	float R;
	bool Intersect(Entity* entity);
	BoundingBox CalcIntersection(QuadTreeNode* node);
};

struct QuadTreeNode
{
	float x;
	float y;
	float w;
	float h;
	QuadTreeNode* nodes[4];
	std::vector<Entity*> entities;

	QuadTreeNode();
	QuadTreeNode(float x, float y, float w, float h);
	~QuadTreeNode();

	void Clear();
	void Clean();
	void Insert(Entity* entity);
	void Remove(Entity* entity);

	bool Contain(Entity* entity);
	bool Contain(Camera* camera);
	bool Contain(BoundingBox boundingBox);
	bool Intersect(Entity* entity);
	bool Intersect(Camera* camera);
	bool Intersect(BoundingBox boundingBox);

	void Retrieve(Entity* entity, float rx, float ry, std::unordered_map<Entity*, QuadTreeNode*>& result);
	void Retrieve(Camera* camera, std::unordered_map<Entity*, QuadTreeNode*>& result);
	void Retrieve(BoundingBox boundingBox, std::unordered_map<Entity*, QuadTreeNode*>& result);

	// Zero-allocation vector query overloads
	void Retrieve(const Space::AABB& box, std::vector<Entity*>& result);
	void Retrieve(const Camera* camera, std::vector<Entity*>& result);

	float GetB() const;
	float GetT() const;
	float GetL() const;
	float GetR() const;
	bool IsSmallestNode() const;

	Space::TLAS* GetTLAS() const { return this->_tlas.get(); }

	static QuadTreeNode* New(float x, float y, float w, float h);
	static bool Update(QuadTreeNode* root, const std::unordered_map<Entity*, QuadTreeNode*>& result);

private:
	std::unique_ptr<Space::TLAS> _tlas;
};
