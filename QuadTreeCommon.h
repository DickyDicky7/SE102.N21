#pragma once

//libs
#include <algorithm>
#include <any>
#include <array>
//header files
#include "Entity.h"

#define MAX_LEVEL 5

namespace QuadTreeRect
{
	struct QuadTreePos
	{
		float x;
		float y;
	};

	struct QuadTreeSize
	{
		float w; // width
		float h; // height
	};

	struct QTRect
	{
		QuadTreePos pos;
		QuadTreeSize size;

		QTRect(const QuadTreePos& pos = { 0.0f, 0.0f }, const QuadTreeSize& size = { 0.0f, 0.0f }) : pos(pos), size(size) {}

		constexpr bool contains(QTRect& r) const //contain whole or a part
		{
			if ((r.size.h * r.size.w) <= 0 && (r.size.h * r.size.w) >= 0)
				return false;

			return !(
				r.pos.x + r.size.w < pos.x ||
				r.pos.x > pos.x + size.w ||
				r.pos.y + r.size.h < pos.y ||
				r.pos.y > pos.y + size.h
				);
		}
	};

	QTRect ConvertEntityRectToQTRect(Entity* _target)
	{
		QTRect _targetSize = {
			{_target->GetX() - _target->GetW() / 2.0f, _target->GetY()},
			{_target->GetW(), _target->GetH()}
		};

		return _targetSize;
	}
};


using QuadTreeList = std::list<Entity*>;

struct QuadTreeContainerItemLocation
{
	typename QuadTreeList* container;
	typename QuadTreeList::iterator iterator;
};

struct QuadTreeContainerItem
{
	Entity* item;
	QuadTreeContainerItemLocation pItem;
};

using QuadTreeContainerList = std::list<QuadTreeContainerItem>;

