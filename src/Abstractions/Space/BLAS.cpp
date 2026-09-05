#include "BLAS.h"
#include "Entity.h"
#include "Camera.h"

namespace Space
{
	AABB AABB::FromEntity(const Entity* entity) noexcept
	{
		if (!entity)
		{
			return AABB(0.0f, 0.0f, 0.0f, 0.0f);
		}
		return AABB(entity->GetL(), entity->GetB(), entity->GetR(), entity->GetT());
	}

	AABB AABB::FromCamera(const Camera* camera) noexcept
	{
		if (!camera)
		{
			return AABB(0.0f, 0.0f, 0.0f, 0.0f);
		}
		return AABB(camera->GetL(), camera->GetB(), camera->GetR(), camera->GetT());
	}

	BLAS::BLAS(Entity* owner)
	{
		this->Build(owner);
	}

	void BLAS::Build(Entity* owner)
	{
		this->_owner = owner;
		this->_primitives.clear();
		this->_isComposite = false;

		if (!owner)
		{
			this->_worldAABB = AABB(0.0f, 0.0f, 0.0f, 0.0f);
			return;
		}

		// Collect collision sub-primitives from composite entities
		owner->ForEachCollisionEntity([this](Entity* subEntity) {
			if (subEntity)
			{
				this->_primitives.push_back({ subEntity, AABB::FromEntity(subEntity) });
			}
		});

		if (this->_primitives.size() > 1)
		{
			this->_isComposite = true;
			this->_worldAABB = AABB::Inverted();
			for (const auto& prim : this->_primitives)
			{
				this->_worldAABB = this->_worldAABB.Union(prim.aabb);
			}
		}
		else
		{
			this->_isComposite = false;
			this->_worldAABB = AABB::FromEntity(owner);
			if (this->_primitives.empty())
			{
				this->_primitives.push_back({ owner, this->_worldAABB });
			}
			else
			{
				this->_primitives[0].aabb = this->_worldAABB;
			}
		}
	}

	void BLAS::Refit()
	{
		if (!this->_owner)
		{
			return;
		}

		if (!this->_isComposite)
		{
			this->_worldAABB = AABB::FromEntity(this->_owner);
			if (!this->_primitives.empty())
			{
				this->_primitives[0].aabb = this->_worldAABB;
			}
			return;
		}

		this->_worldAABB = AABB::Inverted();
		for (auto& prim : this->_primitives)
		{
			if (prim.entity)
			{
				prim.aabb = AABB::FromEntity(prim.entity);
				this->_worldAABB = this->_worldAABB.Union(prim.aabb);
			}
		}
	}
}
