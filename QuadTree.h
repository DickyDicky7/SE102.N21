#pragma once
#include "QuadTreeCommon.h"

class QuadTree
{
protected:
	QuadTreeRect::QTRect region;
	int level;
	QuadTreeList entityList = {};
	std::array<QuadTree*, 4> childs{};
	std::array<QuadTreeRect::QTRect, 4> rChilds{};

public:
	QuadTree()
	{
		level = 0;
		childs = {};
	};
	QuadTree(QuadTreeRect::QTRect _region, int level)
	{
		region = _region;


		level = level;
		entityList = {};
		childs = {};

		QuadTreeRect::QuadTreeSize childSize = {
			_region.size.w / 2.0f,
			_region.size.h / 2.0f,
		};

		rChilds = {
			// Top Left
			QuadTreeRect::QTRect({_region.pos.x, _region.pos.y + childSize.h}, childSize),
			// Top Right
			QuadTreeRect::QTRect({_region.pos.x + childSize.w, _region.pos.y + childSize.h}, childSize),
			// Bottom Left
			QuadTreeRect::QTRect(_region.pos, childSize),
			// Bottom Right
			QuadTreeRect::QTRect({_region.pos.x + childSize.w, _region.pos.y}, childSize)
		};
	};
	~QuadTree()
	{

	};

	void Remove(typename QuadTreeList::iterator it)
	{
		entityList.erase(it);
		return;
	};

	void GetEntityList(typename QuadTreeList& resultList)
	{
		for (auto it = entityList.begin(); it != entityList.end(); it++)
		{
			resultList.push_back(*it);
		}

		for (int i = 0; i < 4; i++)
		{
			if (childs[i])
			{
				childs[i]->GetEntityList(resultList);
			}
		}
		return;
	}

	void GetCurrentNodeEntityCollision(Entity* _target, QuadTreeRect::QTRect _rect, typename QuadTreeList& resultList)
	{
		for (int i = 0; i < 4; i++)
		{
			if (rChilds[i].contains(_rect))
			{
				//_target->LogName();
				//_RPT1(0, "W: %f ;\n H: %f ;\n X: %f ;\n Y: %f ;\n", rChilds[i].size.w, rChilds[i].size.h, rChilds[i].pos.x, rChilds[i].pos.y);
				//_RPT1(0, "W: %f ;\n H: %f ;\n X: %f ;\n Y: %f ;\n", _rect.size.w, _rect.size.h, _rect.pos.x, _rect.pos.y);
				if (childs[i])
				{
					childs[i]->GetEntityList(resultList);
					return;
				}
			}
		}

		//GetEntityList(resultList);
		return;
	};

	QuadTreeContainerItemLocation Insert(Entity* _target, QuadTreeRect::QTRect _rect)
	{
		for (int i = 0; i < 4; i++)
		{
			if (rChilds[i].contains(_rect))
			{
				if (level + 1 < MAX_LEVEL)
				{
					if (!childs[i])
					{
						childs[i] = new QuadTree(rChilds[i], level + 1);;
					}
					return childs[i]->Insert(_target, _rect);
				}
			}
		}

		entityList.push_back(_target);

		return { &entityList, std::prev(entityList.end()) };
	}
};

