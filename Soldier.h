#pragma once
#include "Bill.h"
#include "Enemy.h"
#include "State.h"
#include "Common.h"
#include "Entity.h"
#include "HasSprites.h"
#include "HasTextures.h"
#include "HasAnimations.h"

class SoldierState;
class SoldierRunState;
class SoldierJumpState;
class SoldierShootState;
class SoldierLayDownState;
class SoldierDieState;

class Soldier : public Entity, public Enemy<Bill>
		      , public HasTextures<Soldier>, public HasSprites<Soldier>, public HasAnimations<Soldier>, public CollidableEntity, public HasWeapons
{
public:

	Soldier();
	virtual ~Soldier();
	void SetState(SoldierState*);
	void Update() override;
	void Render() override;
	void HandleInput(Input&) override;

	void LoadSprites() override;
	void LoadTextures() override;
	void LoadAnimations() override;

	SoldierState* GetState();
	void SetState(SoldierState*);
	void GoDead();
	BOOL shootable;
	void Fire() override;
	void StaticResolveNoCollision() override;
	void StaticResolveOnCollision(AABBSweepResult) override;
	void DynamicResolveNoCollision() override;
	void DynamicResolveOnCollision(AABBSweepResult) override;

protected:

	SoldierState* state;
	SoldierState* updateState;
	SoldierState* handleInputState;
};

// build state of soldier
class SoldierState : public State<SoldierState, Soldier>
{

public:

	SoldierState();
	virtual ~SoldierState();

	virtual void Exit(Soldier&) override = 0;
	virtual void Enter(Soldier&) override = 0;
	virtual void Render(Soldier&) override = 0;

	virtual SoldierState* Update(Soldier&) override = 0;
	virtual SoldierState* HandleInput(Soldier&, Input&) override = 0;

protected:

	FLOAT time;

};

class SoldierRunState : public SoldierState
{

public:

	SoldierRunState();
	virtual ~SoldierRunState();

	virtual void Exit(Soldier&) override;
	virtual void Enter(Soldier&) override;
	virtual void Render(Soldier&) override;

	virtual SoldierState* Update(Soldier&) override;
	virtual SoldierState* HandleInput(Soldier&, Input&) override;

};

class SoldierJumpState : public SoldierState
{

public:

	SoldierJumpState();
	virtual ~SoldierJumpState();

	virtual void Exit(Soldier&) override;
	virtual void Enter(Soldier&) override;
	virtual void Render(Soldier&) override;

	virtual SoldierState* Update(Soldier&) override;
	virtual SoldierState* HandleInput(Soldier&, Input&) override;

protected:

	BOOL hasMovedLeft;
	BOOL hasMovedRight;

};

class SoldierShootState : public SoldierState
{

public:

	SoldierShootState();
	virtual ~SoldierShootState();

	virtual void Exit(Soldier&) override;
	virtual void Enter(Soldier&) override;
	virtual void Render(Soldier&) override;

	virtual SoldierState* Update(Soldier&) override;
	virtual SoldierState* HandleInput(Soldier&, Input&) override;

};

class SoldierLayDownState : public SoldierState
{

public:

	SoldierLayDownState();
	virtual ~SoldierLayDownState();

	virtual void Exit(Soldier&) override;
	virtual void Enter(Soldier&) override;
	virtual void Render(Soldier&) override;

	virtual SoldierState* Update(Soldier&) override;
	virtual SoldierState* HandleInput(Soldier&, Input&) override;

};

class SoldierDieState : public SoldierState
{

public:

	SoldierDieState();
	virtual ~SoldierDieState();

	virtual void Exit(Soldier&) override;
	virtual void Enter(Soldier&) override;
	virtual void Render(Soldier&) override;

	virtual SoldierState* Update(Soldier&) override;
	virtual SoldierState* HandleInput(Soldier&, Input&) override;

};