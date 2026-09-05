#pragma once
#include "State.h"
#include "Common.h"
#include "Entity.h"
#include "HasAnimations.h"
#include "HasSprites.h"
#include "HasTextures.h"
#include "Bill.h"
#include "Enemy.h"
#include "ItemCommon.h"

class Item : public Entity
	, public HasTextures<Item>, public HasSprites<Item>, public HasAnimations<Item>, public CollidableEntity
{
public:
	ITEM_TYPE GetItemType() const { return this->_type; }

	Item(ITEM_TYPE type);
	virtual ~Item();
	void Update() override;
	void Render() override;
	void HandleInput(Input& input) override;

	void LoadSprites() override;
	void LoadTextures() override;
	void LoadAnimations() override;

	CollidableEntity* AsCollidable() override { return this; }
	void  StaticResolveNoCollision() override;
	void  StaticResolveOnCollision(AABBSweepResult aabbSweepResult) override;
	void DynamicResolveNoCollision() override;
	void DynamicResolveOnCollision(AABBSweepResult aabbSweepResult) override;

	bool OnBillCollision(Bill& bill, const AABBSweepResult& result) override;
protected:

	ITEM_TYPE _type;

	float _time;
	float _dt;
	float _v0;
	float _theta;

	// _time:  elapsed time
	// _dt:    delta time ( t = t + dt )
	// _v0:    initial throw velocity
	// _theta: throw angle (degrees)

	bool _stopUpdate;
};
