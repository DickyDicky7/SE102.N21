#pragma once

#include "QuadTree.h"	

class QuadTreeContainer
{
protected:
	QuadTree tree;
	QuadTreeContainerList itemList;

public:
	QuadTreeContainer()
	{
	}

	QuadTreeContainer(QuadTreeRect::QTRect _region)
	{
		tree = QuadTree(_region, 0);
	};

	~QuadTreeContainer()
	{

	};

	typename QuadTreeContainerList::iterator begin()
	{
		return itemList.begin();
	}

	typename QuadTreeContainerList::iterator end()
	{
		return itemList.end();
	}

	typename QuadTreeContainerList::const_iterator cbegin()
	{
		return itemList.cbegin();
	}

	typename QuadTreeContainerList::const_iterator cend()
	{
		return itemList.cend();
	}


	void Insert(Entity* _target)
	{
		QuadTreeContainerItem item;
		item.item = _target;

		itemList.push_back(item);

		QuadTreeRect::QTRect _target_size = QuadTreeRect::ConvertEntityRectToQTRect(_target);

		itemList.back().pItem = tree.Insert(_target, _target_size);
	};

	void Remove(typename QuadTreeContainerList::iterator& it)
	{
		itemList.erase(it);
		it->pItem.container->erase(it->pItem.iterator);
	};

	void Relocate(typename QuadTreeContainerList::iterator& it)
	{
		it->pItem.container->erase(it->pItem.iterator);

		QuadTreeRect::QTRect _target_size = QuadTreeRect::ConvertEntityRectToQTRect(it->item);

		it->pItem = tree.Insert(it->item, _target_size);
	};

	std::list<Entity*> GetCollisionWithTarget(Entity* _target)
	{
		QuadTreeContainerList::iterator targetIt;

		for (auto it = itemList.begin(); it != itemList.end(); it++)
		{
			targetIt = it;
		}

		typename QuadTreeList resultList;

		tree.GetCurrentNodeEntityCollision(targetIt->pItem.container, targetIt->pItem.iterator, resultList);

		return resultList;
	}
};
