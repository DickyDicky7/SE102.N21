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
class ExplosionType3State;
class ExplosionDrownState;
// Explosion types:
// type 1: for human-shaped entities, e.g. soldier, scuba soldier, rifleMan, ...
// type 2: for all remaining object-shaped entities
// type 3: for boss 1 and boss 2

class Explosion : public Entity
	, public HasTextures<Explosion>, public HasSprites<Explosion>, public HasAnimations<Explosion>
{
public:
	Explosion();
	Explosion(ExplosionState* state);
	virtual ~Explosion();
	void Update() override;
	void Render() override;
	void HandleInput(Input& input) override;

	void LoadSprites() override;
	void LoadTextures() override;
	void LoadAnimations() override;

protected:
	ExplosionState* _state;
	ExplosionState* _updateState;
	ExplosionState* _handleInputState;
};



class ExplosionState : public State<ExplosionState, Explosion>
{

public:

	ExplosionState();
	virtual ~ExplosionState();

	virtual void Exit(Explosion& explosion) override = 0;
	virtual void Enter(Explosion& explosion) override = 0;
	virtual void Render(Explosion& explosion) override = 0;

	virtual ExplosionState* Update(Explosion& explosion) override = 0;
	virtual ExplosionState* HandleInput(Explosion& explosion, Input& input) override = 0;
};


class ExplosionType1State : public ExplosionState
{

public:

	ExplosionType1State();
	virtual ~ExplosionType1State();

	virtual void Exit(Explosion& explosion) override;
	virtual void Enter(Explosion& explosion) override;
	virtual void Render(Explosion& explosion) override;

	virtual ExplosionState* Update(Explosion& explosion) override;
	virtual ExplosionState* HandleInput(Explosion& explosion, Input& input) override;
};


class ExplosionType2State : public ExplosionState
{

public:

	ExplosionType2State();
	virtual ~ExplosionType2State();

	virtual void Exit(Explosion& explosion) override;
	virtual void Enter(Explosion& explosion) override;
	virtual void Render(Explosion& explosion) override;

	virtual ExplosionState* Update(Explosion& explosion) override;
	virtual ExplosionState* HandleInput(Explosion& explosion, Input& input) override;
};

class ExplosionType3State : public ExplosionState
{

public:

	ExplosionType3State();
	virtual ~ExplosionType3State();

	virtual void Exit(Explosion& explosion) override;
	virtual void Enter(Explosion& explosion) override;
	virtual void Render(Explosion& explosion) override;

	virtual ExplosionState* Update(Explosion& explosion) override;
	virtual ExplosionState* HandleInput(Explosion& explosion, Input& input) override;
};


class ExplosionDrownState : public ExplosionState
{

public:

	ExplosionDrownState();
	virtual ~ExplosionDrownState();

	virtual void Exit(Explosion& explosion) override;
	virtual void Enter(Explosion& explosion) override;
	virtual void Render(Explosion& explosion) override;

	virtual ExplosionState* Update(Explosion& explosion) override;
	virtual ExplosionState* HandleInput(Explosion& explosion, Input& input) override;

protected:

	ULONGLONG _time;

};

