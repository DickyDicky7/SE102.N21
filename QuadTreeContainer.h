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

		QuadTreeRect::QTRect _targetSize = QuadTreeRect::ConvertEntityRectToQTRect(_target);

		itemList.back().pItem = tree.Insert(_target, _targetSize);
	};

	void Remove(typename QuadTreeContainerList::iterator& it)
	{
		itemList.erase(it);
		it->pItem.container->erase(it->pItem.iterator);
	};

	void Relocate(typename QuadTreeContainerList::iterator& it)
	{
		it->pItem.container->erase(it->pItem.iterator);

		QuadTreeRect::QTRect _targetSize = QuadTreeRect::ConvertEntityRectToQTRect(it->item);

		it->pItem = tree.Insert(it->item, _targetSize);
	};

	std::list<Entity*> GetCollisionWithTarget(Entity* _target)
	{
		QuadTreeRect::QTRect _targetSize = QuadTreeRect::ConvertEntityRectToQTRect(_target);

		typename QuadTreeList resultList;

		tree.GetCurrentNodeEntityCollision(_target, _targetSize, resultList);

		return resultList;
	}
};
