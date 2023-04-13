#pragma once
#include "Enemy.h"
#include "State.h"
#include "Common.h"
#include "Entity.h"
#include "HasSprites.h"
#include "HasTextures.h"
#include "HasAnimations.h"

class ScubaSoldierState;
class ScubaSoldierHiddenState;
class ScubaSoldierShootingState;

class ScubaSoldier : public Entity<ScubaSoldier>, public Enemy<ScubaSoldier, Bill>
				   , public HasTextures<ScubaSoldier>, public HasSprites<ScubaSoldier>, public HasAnimations<ScubaSoldier>
{
public:

	ScubaSoldier();
	virtual ~ScubaSoldier();
	void Update() override;
	void Render() override;
	void HandleInput(Input&) override;

	void LoadSprites() override;
	void LoadTextures() override;
	void LoadAnimations() override;
protected:

	ScubaSoldierState* state;
	ScubaSoldierState* updateState;
	ScubaSoldierState* handleInputState;
};

// build state of soldier
class ScubaSoldierState : public State<ScubaSoldierState, ScubaSoldier>
{

public:

	ScubaSoldierState();
	virtual ~ScubaSoldierState();

	virtual void Exit(ScubaSoldier&) override = 0;
	virtual void Enter(ScubaSoldier&) override = 0;
	virtual void Render(ScubaSoldier&) override = 0;

	virtual ScubaSoldierState* Update(ScubaSoldier&) override = 0;
	virtual ScubaSoldierState* HandleInput(ScubaSoldier&, Input&) override = 0;

protected:

	FLOAT time;

};

// shooting state
class ScubaSoldierShootingState : public ScubaSoldierState
{

public:

	ScubaSoldierShootingState();
	virtual ~ScubaSoldierShootingState();

	virtual void Exit(ScubaSoldier&) override;
	virtual void Enter(ScubaSoldier&) override;
	virtual void Render(ScubaSoldier&) override;

	virtual ScubaSoldierState* Update(ScubaSoldier&) override;
	virtual ScubaSoldierState* HandleInput(ScubaSoldier&, Input&) override;

};

// hidden state
class ScubaSoldierHiddenState : public ScubaSoldierState
{

public:

	ScubaSoldierHiddenState();
	virtual ~ScubaSoldierHiddenState();

	virtual void Exit(ScubaSoldier&) override;
	virtual void Enter(ScubaSoldier&) override;
	virtual void Render(ScubaSoldier&) override;

	virtual ScubaSoldierState* Update(ScubaSoldier&) override;
	virtual ScubaSoldierState* HandleInput(ScubaSoldier&, Input&) override;

};
