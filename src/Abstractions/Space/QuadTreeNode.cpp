#include "QuadTreeNode.h"
#include <algorithm>

QuadTreeNode::QuadTreeNode()
	: x(0.0f), y(0.0f), w(0.0f), h(0.0f)
	, nodes{ nullptr, nullptr, nullptr, nullptr }
	, _tlas(std::make_unique<Space::TLAS>())
{
}

QuadTreeNode::QuadTreeNode(float x, float y, float w, float h)
	: x(x), y(y), w(w), h(h)
	, nodes{ nullptr, nullptr, nullptr, nullptr }
	, _tlas(std::make_unique<Space::TLAS>())
{
}

QuadTreeNode::~QuadTreeNode()
{
	this->Clear();
}

void QuadTreeNode::Clear()
{
	if (this->_tlas)
	{
		this->_tlas->Clear();
	}
	for (int i = 0; i < 4; ++i)
	{
		if (this->nodes[i])
		{
			delete this->nodes[i];
			this->nodes[i] = nullptr;
		}
	}
	this->entities.clear();
}

void QuadTreeNode::Clean()
{
	if (this->_tlas)
	{
		this->_tlas->Clean();
	}
	else
	{
		for (auto& entity : this->entities)
		{
			Destroy(entity);
		}
	}
	for (int i = 0; i < 4; ++i)
	{
		if (this->nodes[i])
		{
			this->nodes[i]->Clean();
			delete this->nodes[i];
			this->nodes[i] = nullptr;
		}
	}
	this->entities.clear();
}

void QuadTreeNode::Insert(Entity* entity)
{
	if (!entity)
	{
		return;
	}

	this->entities.push_back(entity);
	if (this->_tlas)
	{
		this->_tlas->Insert(entity);
	}
}

void QuadTreeNode::Remove(Entity* entity)
{
	if (!entity)
	{
		return;
	}

	std::erase(this->entities, entity);
	if (this->_tlas)
	{
		this->_tlas->Remove(entity);
	}
}

bool QuadTreeNode::Contain(Entity* entity)
{
	if (!entity) return false;
	return entity->GetB() >= this->GetB()
		&& entity->GetT() <= this->GetT()
		&& entity->GetL() >= this->GetL()
		&& entity->GetR() <= this->GetR();
}

bool QuadTreeNode::Contain(Camera* camera)
{
	if (!camera) return false;
	return camera->GetB() >= this->GetB()
		&& camera->GetT() <= this->GetT()
		&& camera->GetL() >= this->GetL()
		&& camera->GetR() <= this->GetR();
}

bool QuadTreeNode::Contain(BoundingBox boundingBox)
{
	return boundingBox.B >= this->GetB()
		&& boundingBox.T <= this->GetT()
		&& boundingBox.L >= this->GetL()
		&& boundingBox.R <= this->GetR();
}

bool QuadTreeNode::Intersect(Entity* entity)
{
	if (!entity) return false;
	return !(entity->GetB() >= this->GetT()
		||   entity->GetT() <= this->GetB()
		||   entity->GetL() >= this->GetR()
		||   entity->GetR() <= this->GetL());
}

bool QuadTreeNode::Intersect(Camera* camera)
{
	if (!camera) return false;
	return !(camera->GetB() >= this->GetT()
		||   camera->GetT() <= this->GetB()
		||   camera->GetL() >= this->GetR()
		||   camera->GetR() <= this->GetL());
}

bool QuadTreeNode::Intersect(BoundingBox boundingBox)
{
	return !(boundingBox.B >= this->GetT()
		||   boundingBox.T <= this->GetB()
		||   boundingBox.L >= this->GetR()
		||   boundingBox.R <= this->GetL());
}

void QuadTreeNode::Retrieve(Entity* entity, float rx, float ry, std::unordered_map<Entity*, QuadTreeNode*>& result)
{
	if (!entity || !this->_tlas) return;
	const Space::AABB box = Space::AABB::FromEntity(entity).Expand(rx, ry);
	this->_tlas->QueryAABB(box, [&](Entity* found) {
		result.insert({ found, this });
	});
}

void QuadTreeNode::Retrieve(Camera* camera, std::unordered_map<Entity*, QuadTreeNode*>& result)
{
	if (!camera || !this->_tlas) return;
	this->_tlas->QueryFrustum(camera, [&](Entity* found) {
		result.insert({ found, this });
	});
}

void QuadTreeNode::Retrieve(BoundingBox boundingBox, std::unordered_map<Entity*, QuadTreeNode*>& result)
{
	if (!this->_tlas) return;
	const Space::AABB box(boundingBox.L, boundingBox.B, boundingBox.R, boundingBox.T);
	this->_tlas->QueryAABB(box, [&](Entity* found) {
		result.insert({ found, this });
	});
}

void QuadTreeNode::Retrieve(const Space::AABB& box, std::vector<Entity*>& result)
{
	if (!this->_tlas) return;
	this->_tlas->QueryAABB(box, [&](Entity* found) {
		result.push_back(found);
	});
}

void QuadTreeNode::Retrieve(const Camera* camera, std::vector<Entity*>& result)
{
	if (!camera || !this->_tlas) return;
	this->_tlas->QueryFrustum(camera, [&](Entity* found) {
		result.push_back(found);
	});
}

float QuadTreeNode::GetB() const
{
	return this->y;
}

float QuadTreeNode::GetT() const
{
	return this->y + this->h;
}

float QuadTreeNode::GetL() const
{
	return this->x;
}

float QuadTreeNode::GetR() const
{
	return this->x + this->w;
}

bool QuadTreeNode::IsSmallestNode() const
{
	return this->w <= Constants::Physics::QUADTREE_NODE_SMALLEST_WIDTH
		&& this->h <= Constants::Physics::QUADTREE_NODE_SMALLEST_HEIGHT;
}

QuadTreeNode* QuadTreeNode::New(float x, float y, float w, float h)
{
	return new QuadTreeNode(x, y, w, h);
}

bool QuadTreeNode::Update(QuadTreeNode* root, const std::unordered_map<Entity*, QuadTreeNode*>& result)
{
	if (!root || !root->_tlas)
	{
		return false;
	}

	root->_tlas->Update();
	return true;
}

bool BoundingBox::Intersect(Entity* entity)
{
	if (!entity) return false;
	return !(entity->GetB() >= this->T
		||   entity->GetT() <= this->B
		||   entity->GetL() >= this->R
		||   entity->GetR() <= this->L);
}

BoundingBox BoundingBox::CalcIntersection(QuadTreeNode* node)
{
	if (!node) return *this;
	return BoundingBox{
		(std::max)(this->B, node->GetB()),
		(std::min)(this->T, node->GetT()),
		(std::max)(this->L, node->GetL()),
		(std::min)(this->R, node->GetR())
	};
}
