#include "QTreeNode.h"

QTreeNode::QTreeNode(RECT* _region, int level)
{
	region = _region;
	level = level;
	entity_list = {};
	childs = NULL;
}

QTreeNode::~QTreeNode()
{

}

void QTreeNode::Split()
{
	childs = new QTreeNode * [4];

	int _halfWidth = region->right - region->left;
	int _halfHeight = region->top - region->bottom;

	childs[0] = new QTreeNode(new RECT({ region->top, region->left, region->left + _halfWidth, region->top + _halfHeight }), level + 1);
	childs[1] = new QTreeNode(new RECT({ region->top, region->left + _halfWidth, region->right, region->top + _halfHeight }), level + 1);
	childs[2] = new QTreeNode(new RECT({ region->top + _halfHeight , region->left + _halfWidth, region->right, region->bottom }), level + 1);
	childs[3] = new QTreeNode(new RECT({ region->top + _halfHeight , region->left, region->left + _halfWidth, region->bottom }), level + 1);
}

void QTreeNode::Insert(Entity<std::any>* _target)
{
	if (childs)
	{
		std::for_each(childs, childs + 4, [_target](QTreeNode* eachChild) -> void {
			if (eachChild->IsContain(_target))
				eachChild->Insert(_target);
			});
		return;
	}

	if (IsContain(_target))
	{
		entity_list.push_back(_target);
	}

	if (entity_list.size() > MAX_OBJECT_IN_REGION && level < MAX_LEVEL)
	{
		Split();

		while (!entity_list.empty())
		{
			std::for_each(childs, childs + 4, [&](QTreeNode* eachChild) -> void {
				if (eachChild->IsContain(entity_list.back()))
					eachChild->Insert(entity_list.back());
				});

			entity_list.pop_back();
		}
	}

}

bool QTreeNode::IsContain(Entity<std::any>* _target)
{
	// can not get width and height of _target 
	return true;
}

void QTreeNode::GetAllCollisionWithTarget(Entity<std::any>* _target, std::vector<Entity<std::any>*>* _returnValueVector)
{
	if (childs)
	{
		std::for_each(childs, childs + 4, [_target, _returnValueVector](QTreeNode* eachChild) -> void {
			if (eachChild->IsContain(_target))
				eachChild->GetAllCollisionWithTarget(_target, _returnValueVector);
			});
		return;
	}

	if (IsContain(_target))
	{
		std::for_each(entity_list.begin(), entity_list.end(), [_target, _returnValueVector](Entity<std::any>* entity) -> void {
			if (entity != _target)
				_returnValueVector->push_back(entity);
			});
	}
}