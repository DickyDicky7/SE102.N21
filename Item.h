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
	ITEM_TYPE type;

	Item(ITEM_TYPE);
	virtual ~Item();
	void Update() override;
	void Render() override;
	void HandleInput(Input&) override;

	void LoadSprites() override;
	void LoadTextures() override;
	void LoadAnimations() override;

	void  StaticResolveNoCollision() override;
	void  StaticResolveOnCollision(AABBSweepResult) override;
	void DynamicResolveNoCollision() override;
	void DynamicResolveOnCollision(AABBSweepResult) override;

protected:
	

	FLOAT time;
	FLOAT dt;
	FLOAT v0;
	FLOAT θ;

	// time: thời gian
	// dt: delta time ( t = t + dt )
	// v0: vận tốc ném viên đạn
	// θ: góc ném

	BOOL stopUpdate;
};