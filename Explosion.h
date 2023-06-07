#pragma once
#include "State.h"
#include "Common.h"
#include "Entity.h"
#include "HasAnimations.h"
#include "HasSprites.h"
#include "HasTextures.h"
#include "Bill.h"
#include "Enemy.h"
#include "AirCraft.h"

class Explosion;
class ExplosionState;
class ExplosionType1State;
class ExplosionType2State;

// explosion co 2 type
// type 1: danh cho cac enity dang nguoi nhu: soldier, scuba soldier, rifleMan,...
// type 2: danh cho cac enity dang vat the con lai

class Explosion : public Entity
	, public HasTextures<Explosion>, public HasSprites<Explosion>, public HasAnimations<Explosion>
{
public:
	Explosion();
	virtual ~Explosion();
	void Update() override;
	void Render() override;
	void HandleInput(Input&) override;

	void LoadSprites() override;
	void LoadTextures() override;
	void LoadAnimations() override;

protected:
	ExplosionState* state;
	ExplosionState* updateState;
	ExplosionState* handleInputState;
};



class ExplosionState : public State<ExplosionState, Explosion>
{

public:

	ExplosionState();
	virtual ~ExplosionState();

	virtual void Exit(Explosion&) override = 0;
	virtual void Enter(Explosion&) override = 0;
	virtual void Render(Explosion&) override = 0;

	virtual ExplosionState* Update(Explosion&) override = 0;
	virtual ExplosionState* HandleInput(Explosion&, Input&) override = 0;
};


class ExplosionType1State : public ExplosionState
{

public:

	ExplosionType1State();
	virtual ~ExplosionType1State();

	virtual void Exit(Explosion&) override;
	virtual void Enter(Explosion&) override;
	virtual void Render(Explosion&) override;

	virtual ExplosionState* Update(Explosion&) override;
	virtual ExplosionState* HandleInput(Explosion&, Input&) override;
protected:
	FLOAT delayTime;
};


class ExplosionType2State : public ExplosionState
{

public:

	ExplosionType2State();
	virtual ~ExplosionType2State();

	virtual void Exit(Explosion&) override;
	virtual void Enter(Explosion&) override;
	virtual void Render(Explosion&) override;

	virtual ExplosionState* Update(Explosion&) override;
	virtual ExplosionState* HandleInput(Explosion&, Input&) override;
protected:
	FLOAT delayTime;
};