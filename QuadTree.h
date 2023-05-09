#pragma once
#include "QuadTreeCommon.h"

void print(float value)
{
	char buffer[64]; // Increase buffer size to accommodate the additional newline character
	_snprintf_s(buffer, sizeof(buffer), _TRUNCATE, "%f\n", value); // Add "\n" to the format string
	OutputDebugStringA(buffer);
}

class QuadTree
{
protected:
	QuadTreeRect::QTRect region;
	int level;
	QuadTreeList entityList = {};
	std::array<QuadTree*, 4> childs{ NULL, NULL ,NULL ,NULL };
	std::array<QuadTreeRect::QTRect, 4> rChilds{};

public:
	QuadTree()
	{
		level = 5;
		childs = {};
	};
	QuadTree(QuadTreeRect::QTRect _region, int _level)
	{
		region = _region;
		level = _level;
		entityList = {};
		childs = { NULL, NULL ,NULL ,NULL };

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

				if (childs[i])
				{
					childs[i]->GetEntityList(resultList);
					return;
				}
			}
		}

		GetEntityList(resultList);
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
						childs[i] = new QuadTree(rChilds[i], level + 1);
					}
					return childs[i]->Insert(_target, _rect);
				}
			}
		}

		entityList.push_back(_target);

		return { &entityList, std::prev(entityList.end()) };
	}
};

