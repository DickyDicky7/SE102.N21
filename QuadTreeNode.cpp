#include "QuadTreeNode.h"


void QuadTreeNode::Clear()
{
	for (int i = 0; i <= 3; i++) if (this->nodes[i]) this->nodes[i]->Clear(), Destroy(this->nodes[i]);
	this->entities.clear();
}

void QuadTreeNode::Clean()
{
	for (int i = 0; i <= 3; i++) if (this->nodes[i]) this->nodes[i]->Clean(), Destroy(this->nodes[i]);
	for (auto& entity : entities) Destroy(entity);
	this->entities.clear();
}

void QuadTreeNode::Insert(Entity* entity)
{
	if (this->Contain(entity))
	{
		if (this->IsSmallestNode())
		{
			this->entities.push_back(entity);
			return;
		}

		if (!this->nodes[0]) this->nodes[0] = new QuadTreeNode{ this->x                 , this->y                 , this->w * 0.5f, this->h * 0.5f };
		if (!this->nodes[1]) this->nodes[1] = new QuadTreeNode{ this->x + this->w * 0.5f, this->y                 , this->w * 0.5f, this->h * 0.5f };
		if (!this->nodes[2]) this->nodes[2] = new QuadTreeNode{ this->x + this->w * 0.5f, this->y + this->h * 0.5f, this->w * 0.5f, this->h * 0.5f };
		if (!this->nodes[3]) this->nodes[3] = new QuadTreeNode{ this->x                 , this->y + this->h * 0.5f, this->w * 0.5f, this->h * 0.5f };

		for (int i = 0; i <= 3; i++)
		{
			if (this->nodes[i]->Contain(entity))
			{
				this->nodes[i]->Insert(entity);
				return;
			}
		}

		this->entities.push_back(entity);
	}
}


void QuadTreeNode::Remove(Entity* entity)
{
}


BOOL QuadTreeNode::Contain(Entity* entity)
{
	return entity->GetB() >= this->GetB()
		&& entity->GetT() <= this->GetT()
		&& entity->GetL() >= this->GetL()
		&& entity->GetR() <= this->GetR()
		;
}

BOOL QuadTreeNode::Contain(Camera* camera)
{
	return camera->GetB() >= this->GetB()
		&& camera->GetT() <= this->GetT()
		&& camera->GetL() >= this->GetL()
		&& camera->GetR() <= this->GetR()
		;
}

BOOL QuadTreeNode::Contain(BoundingBox boundingBox)
{
	return boundingBox.B >= this->GetB()
		&& boundingBox.T <= this->GetT()
		&& boundingBox.L >= this->GetL()
		&& boundingBox.R <= this->GetR()
		;
}

BOOL QuadTreeNode::Intersect(Entity* entity)
{
	return !(entity->GetB() >= this->GetT()
		||   entity->GetT() <= this->GetB()
		||   entity->GetL() >= this->GetR()
		||   entity->GetR() <= this->GetL())
		;
}

BOOL QuadTreeNode::Intersect(Camera* camera)
{
	return !(camera->GetB() >= this->GetT()
		||   camera->GetT() <= this->GetB()
		||   camera->GetL() >= this->GetR()
		||   camera->GetR() <= this->GetL())
		;
}

BOOL QuadTreeNode::Intersect(BoundingBox boundingBox)
{
	return !(boundingBox.B >= this->GetT()
		||	 boundingBox.T <= this->GetB()
		||	 boundingBox.L >= this->GetR()
		||   boundingBox.R <= this->GetL())
		;
}


void QuadTreeNode::Retrieve(Entity* entity, FLOAT rx, FLOAT ry, std::unordered_map<Entity*, QuadTreeNode*>& result)
{
	this->Retrieve({ entity->GetB() - ry, entity->GetT() + ry, entity->GetL() - rx, entity->GetR() + rx }, result);
}

void QuadTreeNode::Retrieve(Camera* camera, std::unordered_map<Entity*, QuadTreeNode*>& result)
{
	this->Retrieve({ camera->GetB(), camera->GetT(), camera->GetL(), camera->GetR() }, result);
}

void QuadTreeNode::Retrieve(BoundingBox boundingBox, std::unordered_map<Entity*, QuadTreeNode*>& result)
{
	if (this->Contain(boundingBox))
	{
		if (this->IsSmallestNode())
		{
			for (auto& entity : entities)
			{
				if (boundingBox.Intersect(entity))
				{
					result.insert({ entity, this });
				}
			}
			return;
		}

		for (auto& entity : entities)
		{
			if (boundingBox.Intersect(entity))
			{
				result.insert({ entity, this });
			}
		}

		for (int i = 0; i <= 3; i++)
		{
			if (this->nodes[i] && this->nodes[i]->Contain(boundingBox))
			{
				this->nodes[i]->Retrieve(boundingBox, result);
				return;
			}
		}


		for (int i = 0; i <= 3; i++)
		{
			if (this->nodes[i] && this->nodes[i]->Intersect(boundingBox))
			{
				this->nodes[i]->Retrieve(boundingBox.CalcIntersection(this), result);
			}
		}
	}
	else
	if (this->Intersect(boundingBox))
	{
		if (this->IsSmallestNode())
		{
			for (auto& entity : entities)
			{
				if (boundingBox.Intersect(entity))
				{
					result.insert({ entity, this });
				}
			}
			return;
		}

		for (auto& entity : entities)
		{
			if (boundingBox.Intersect(entity))
			{
				result.insert({ entity, this });
			}
		}

		for (int i = 0; i <= 3; i++)
		{
			if (this->nodes[i] && this->nodes[i]->Intersect(boundingBox))
			{
				this->nodes[i]->Retrieve(boundingBox.CalcIntersection(this), result);
			}
		}
	}
}


FLOAT QuadTreeNode::GetB() const
{
	return this->y;
}

FLOAT QuadTreeNode::GetT() const
{
	return this->y + this->h;
}

FLOAT QuadTreeNode::GetL() const
{
	return this->x;
}

FLOAT QuadTreeNode::GetR() const
{
	return this->x + this->w;
}

BOOL QuadTreeNode::IsSmallestNode() const
{
	return this->w == NODE_SMALLEST_W
		&& this->h == NODE_SMALLEST_H;
}

QuadTreeNode* QuadTreeNode::New(FLOAT x, FLOAT y, FLOAT w, FLOAT h)
{
	QuadTreeNode* newNode = new QuadTreeNode();
	newNode->w = NODE_SMALLEST_W;
	newNode->h = NODE_SMALLEST_H;
	newNode->x = x;
	newNode->y = y;
	while (newNode->w < w 
	||     newNode->h < h) 
	{
		   newNode->w *= 2;
		   newNode->h *= 2;
	}
	return newNode;
}


BOOL BoundingBox::Intersect(Entity* entity)
{
	return !(entity->GetB() >= this->T
		||   entity->GetT() <= this->B
		||   entity->GetL() >= this->R
		||   entity->GetR() <= this->L)
		;
}

BoundingBox BoundingBox::CalcIntersection(QuadTreeNode* node)
{
	return BoundingBox{ max(this->B, node->GetB()), min(this->T, node->GetT()), max(this->L,node->GetL()), min(this->R, node->GetR()) };
}

