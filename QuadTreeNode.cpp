#include "QuadTreeNode.h"

void QuadTreeNode::Clear()
{
	for (int i = 0; i <= 3; i++) if (this->nodes[i]) this->nodes[i]->Clear(), this->nodes[i]->entities.clear(), Destroy(this->nodes[i]);
	this->entities.clear();
}

void QuadTreeNode::Insert(Entity* entity)
{
	if (!this->Contain(entity)) return;
	if ( this->w == NODE_SMALLEST_W 
	&&   this->h == NODE_SMALLEST_H)
	{
		this->entities.push_back(entity);
		return;
	}
	for (int i = 0; i <= 3; i++) if (this->nodes[i]) this->nodes[i]->Insert(entity);
}

void QuadTreeNode::Remove(Entity* entity)
{
	if (!this->Contain(entity)) return;
	if ( this->w == NODE_SMALLEST_W
	&&   this->h == NODE_SMALLEST_H)
	{
		this->entities.remove(entity);
		return;
	}
	for (int i = 0; i <= 3; i++) if (this->nodes[i]) this->nodes[i]->Remove(entity);
}

BOOL QuadTreeNode::Contain(Entity* entity)
{
	FLOAT selfB = this->y;
	FLOAT selfT = this->y + this->h;
	FLOAT selfL = this->x;
	FLOAT selfR = this->x + this->w;

	FLOAT otherB = entity->GetY();
	FLOAT otherT = entity->GetY() + entity->GetH();
	FLOAT otherL = entity->GetX() - entity->GetW() / 2.0f;
	FLOAT otherR = entity->GetX() + entity->GetW() / 2.0f;

	return
		otherL <= selfR &&
		otherR >= selfL &&
		otherT >= selfB &&
		otherB <= selfT
		;
}

BOOL QuadTreeNode::Contain(Camera* camera)
{
	FLOAT selfB = this->y;
	FLOAT selfT = this->y + this->h;
	FLOAT selfL = this->x;
	FLOAT selfR = this->x + this->w;

	FLOAT otherB = camera->GetY() - SCREEN_HEIGHT / SCALING_RATIO_Y * 0.5f;
	FLOAT otherT = camera->GetY() + SCREEN_HEIGHT / SCALING_RATIO_Y * 0.5f;
	FLOAT otherL = camera->GetX() - SCREEN_WIDTH  / SCALING_RATIO_X * 0.5f;
	FLOAT otherR = camera->GetX() + SCREEN_WIDTH  / SCALING_RATIO_X * 0.5f;

	return
		otherL <= selfR &&
		otherR >= selfL &&
		otherT >= selfB &&
		otherB <= selfT
		;
}

void QuadTreeNode::Retrieve(Entity* entity, std::unordered_set<Entity*>& resultSet)
{
	if (!this->Contain(entity)) return;
	if ( this->w == NODE_SMALLEST_W
	&&   this->h == NODE_SMALLEST_H)
	{
		for (auto& e : entities) if (e != entity) resultSet.insert(e);
		return;
	}
	for (int i = 0; i <= 3; i++) if (this->nodes[i]) this->nodes[i]->Retrieve(entity, resultSet);
}

void QuadTreeNode::Retrieve(Camera* camera, std::unordered_set<Entity*>& resultSet)
{
	if (!this->Contain(camera)) return;
	if ( this->w == NODE_SMALLEST_W
	&&   this->h == NODE_SMALLEST_H)
	{
		for (auto& e : entities) resultSet.insert(e);
		return;
	}
	for (int i = 0; i <= 3; i++) if (this->nodes[i]) this->nodes[i]->Retrieve(camera, resultSet);
}

void QuadTreeNode::DrillDown(QuadTreeNode* node)
{
	if (!node) return;
	if ( node->w == NODE_SMALLEST_W
	&&   node->h == NODE_SMALLEST_H) return;
	node->nodes[0] = new QuadTreeNode{ node->x						      , node->y						       , node->w / (I_DIVISOR + 1) * I_DIVISOR, node->h / (I_DIVISOR + 1) * I_DIVISOR };
	node->nodes[1] = new QuadTreeNode{ node->x + node->w / (I_DIVISOR + 1), node->y						       , node->w / (I_DIVISOR + 1) * I_DIVISOR, node->h / (I_DIVISOR + 1) * I_DIVISOR };
	node->nodes[2] = new QuadTreeNode{ node->x + node->w / (I_DIVISOR + 1), node->y + node->h / (I_DIVISOR + 1), node->w / (I_DIVISOR + 1) * I_DIVISOR, node->h / (I_DIVISOR + 1) * I_DIVISOR };
	node->nodes[3] = new QuadTreeNode{ node->x						      , node->y + node->h / (I_DIVISOR + 1), node->w / (I_DIVISOR + 1) * I_DIVISOR, node->h / (I_DIVISOR + 1) * I_DIVISOR };
	for (int i = 0; i <= 3; i++) if (node->nodes[i]) DrillDown(node->nodes[i]);
}

QuadTreeNode* QuadTreeNode::New(FLOAT x, FLOAT y, FLOAT w, FLOAT h, QuadTreeNode* bottomLeftNode)
{
	QuadTreeNode* newNode = new QuadTreeNode();
	if (!bottomLeftNode)
	{
		newNode->nodes[0] = new QuadTreeNode{ x							     , y			                  , NODE_SMALLEST_W, NODE_SMALLEST_H };
		newNode->nodes[1] = new QuadTreeNode{ x + NODE_SMALLEST_W / I_DIVISOR, y			                  , NODE_SMALLEST_W, NODE_SMALLEST_H };
		newNode->nodes[2] = new QuadTreeNode{ x + NODE_SMALLEST_W / I_DIVISOR, y + NODE_SMALLEST_H / I_DIVISOR, NODE_SMALLEST_W, NODE_SMALLEST_H };
		newNode->nodes[3] = new QuadTreeNode{ x							     , y + NODE_SMALLEST_H / I_DIVISOR, NODE_SMALLEST_W, NODE_SMALLEST_H };
		newNode->x = x;
		newNode->y = y;
		newNode->w = NODE_SMALLEST_W / I_DIVISOR * (I_DIVISOR + 1);
		newNode->h = NODE_SMALLEST_H / I_DIVISOR * (I_DIVISOR + 1);
	}
	else
	{
		if (bottomLeftNode->w >= w && bottomLeftNode->h >= h)
		{
			Destroy(newNode); return bottomLeftNode;
		}
		newNode->nodes[0] = bottomLeftNode;
		newNode->nodes[1] = new QuadTreeNode{ bottomLeftNode->x + bottomLeftNode->w / I_DIVISOR, bottomLeftNode->y					              , bottomLeftNode->w, bottomLeftNode->h };
		newNode->nodes[2] = new QuadTreeNode{ bottomLeftNode->x + bottomLeftNode->w / I_DIVISOR, bottomLeftNode->y + bottomLeftNode->h / I_DIVISOR, bottomLeftNode->w, bottomLeftNode->h };
		newNode->nodes[3] = new QuadTreeNode{ bottomLeftNode->x                                , bottomLeftNode->y + bottomLeftNode->h / I_DIVISOR, bottomLeftNode->w, bottomLeftNode->h };
		newNode->x = bottomLeftNode->x;
		newNode->y = bottomLeftNode->y;
		newNode->w = bottomLeftNode->w / I_DIVISOR * (I_DIVISOR + 1);
		newNode->h = bottomLeftNode->h / I_DIVISOR * (I_DIVISOR + 1);
		for (int i = 1; i <= 3; i++) if (newNode->nodes[i]) DrillDown(newNode->nodes[i]);
	}
	return New(x, y, w, h, newNode);
}
