#pragma once

#include "Common.h"
#include "Entity.h"
#include "Camera.h"

static constexpr FLOAT I_DIVISOR = 1.0f; static constexpr FLOAT S_DIVISOR = 30.0f; static constexpr FLOAT NODE_SMALLEST_W = SCREEN_WIDTH / SCALING_RATIO_X / S_DIVISOR; static constexpr FLOAT NODE_SMALLEST_H = SCREEN_HEIGHT / SCALING_RATIO_Y / S_DIVISOR;

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
	void Retrieve(Entity*, std::unordered_set<Entity*>&);
	void Retrieve(Camera*, std::unordered_set<Entity*>&);

	static void DrillDown(QuadTreeNode*);
	static QuadTreeNode* New(FLOAT, FLOAT, FLOAT, FLOAT, QuadTreeNode* = NULL);

};

