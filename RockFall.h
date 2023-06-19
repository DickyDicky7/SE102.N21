#pragma once
#include "State.h"
#include "Common.h"
#include "Entity.h"
#include "HasAnimations.h"
#include "HasSprites.h"
#include "HasTextures.h"
#include "Bill.h"
#include "Enemy.h"

class RockFall;
class RockFallState;
class RockFallNormalState;
class RockFallFallState;


class RockFall : public Entity, public Enemy<Bill>
	, public HasTextures<RockFall>, public HasSprites<RockFall>, public HasAnimations<RockFall>, public CollidableEntity
{
public:
	RockFall();
	virtual ~RockFall();
	void Update() override;
	void Render() override;
	void HandleInput(Input&) override;

	void LoadSprites() override;
	void LoadTextures() override;
	void LoadAnimations() override;

	void setTimeDelayToFall(FLOAT time)
	{
		_timedelayToFall = time;
	}
	FLOAT getTimeDelayToFall() 
	{
		return _timedelayToFall;
	}

	std::unordered_set<Entity*> alreadyCollidedWithEntities;
	BOOL bouncedBack = 0;
	void  StaticResolveNoCollision() override;
	void  StaticResolveOnCollision(AABBSweepResult) override;
	void DynamicResolveNoCollision() override;
	void DynamicResolveOnCollision(AABBSweepResult) override;
protected:
	RockFallState* state;
	RockFallState* updateState;
	RockFallState* handleInputState;

	FLOAT _timedelayToFall;
};



class RockFallState : public State<RockFallState, RockFall>
{

public:

	RockFallState();
	virtual ~RockFallState();

	virtual void Exit(RockFall&) override = 0;
	virtual void Enter(RockFall&) override = 0;
	virtual void Render(RockFall&) override = 0;

	virtual RockFallState* Update(RockFall&) override = 0;
	virtual RockFallState* HandleInput(RockFall&, Input&) override = 0;

protected:

	FLOAT time;

};


class RockFallNormalState : public RockFallState
{

public:

	RockFallNormalState();
	virtual ~RockFallNormalState();

	virtual void Exit(RockFall&) override;
	virtual void Enter(RockFall&) override;
	virtual void Render(RockFall&) override;

	virtual RockFallState* Update(RockFall&) override;
	virtual RockFallState* HandleInput(RockFall&, Input&) override;
};


class RockFallFallState : public RockFallState
{

public:

	RockFallFallState();
	virtual ~RockFallFallState();

	virtual void Exit(RockFall&) override;
	virtual void Enter(RockFall&) override;
	virtual void Render(RockFall&) override;

	virtual RockFallState* Update(RockFall&) override;
	virtual RockFallState* HandleInput(RockFall&, Input&) override;
};