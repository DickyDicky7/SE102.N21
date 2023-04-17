#pragma once

#include <algorithm>
#include <any>

#include "Entity.h"
#include "Common.h"


#define MAX_LEVEL                5
#define MAX_OBJECT_IN_REGION     4

class QTreeNode
{
private:
	RECT* region;
	int level;
	std::vector<Entity<std::any>*> entity_list;
	QTreeNode** childs;

public:
	QTreeNode(RECT*, int);
	~QTreeNode();

	void Split();

	void Insert(Entity<std::any>*);

	bool IsContain(Entity<std::any>*);

	void GetAllCollisionWithTarget(Entity<std::any>*, std::vector<Entity<std::any>*>*);
};