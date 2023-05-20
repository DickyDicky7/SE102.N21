#pragma once

#include "Common.h"
#include "Entity.h"
#include "Camera.h"

static constexpr INT   MAX    = 20;
static constexpr FLOAT TILE_C = 08.0f;
static constexpr FLOAT TILE_R = 08.0f;
static constexpr FLOAT TILE_W = 16.0f;
static constexpr FLOAT TILE_H = 16.0f;
static constexpr FLOAT NODE_SMALLEST_W = TILE_W * TILE_C;
static constexpr FLOAT NODE_SMALLEST_H = TILE_H * TILE_R;

struct QuadTreeNode;

struct BoundingBox
{

	FLOAT B;
	FLOAT T;
	FLOAT L;
	FLOAT R;
	BOOL Intersect(Entity*);
	BoundingBox CalcIntersection(QuadTreeNode*);

};

struct QuadTreeNode
{

	FLOAT x;
	FLOAT y;
	FLOAT w;
	FLOAT h;
	QuadTreeNode* nodes[4];
	std::list<Entity*> entities;

	void Clear();
	void Clean();
	void Insert(Entity*);
	void Remove(Entity*);

	BOOL Contain(Entity*);
	BOOL Contain(Camera*);
	BOOL Contain(BoundingBox);
	BOOL Intersect(Entity*);
	BOOL Intersect(Camera*);
	BOOL Intersect(BoundingBox);

	void Retrieve(Entity*, FLOAT, FLOAT, std::unordered_map<Entity*, QuadTreeNode*>&);
	void Retrieve(Camera*              , std::unordered_map<Entity*, QuadTreeNode*>&);
	void Retrieve(BoundingBox          , std::unordered_map<Entity*, QuadTreeNode*>&);

	FLOAT GetB() const;
	FLOAT GetT() const;
	FLOAT GetL() const;
	FLOAT GetR() const;
	BOOL IsSmallestNode() const;
	static QuadTreeNode* New(FLOAT, FLOAT, FLOAT, FLOAT);

};

