#pragma once
#include "State.h"
#include "Common.h"
#include "Entity.h"
#include "HasAnimations.h"
#include "HasSprites.h"
#include "HasTextures.h"
#include "Bill.h"
#include "Enemy.h"
#include "RifleMan.h"
#include "HasWeapons.h"

class RifleManHideOnBush;
class RifleManHideOnBushState;

class RifleManHideOnBushHidingState;
class RifleManHideOnBushAppearState;
class RifleManHideOnBushStandingState;
class RifleManHideOnBushHideState;

class RifleManHideOnBush : public Entity, public Enemy<Bill>
	, public HasTextures<RifleManHideOnBush>, public HasSprites<RifleManHideOnBush>, public HasAnimations<RifleManHideOnBush>, public RifleMan, public HasWeapons
{
public:
	RifleManHideOnBush();
	~RifleManHideOnBush();

	virtual void Update() override;
	virtual void Render() override;
	virtual void HandleInput(Input&) override;

	void Fire() override;
	void Fire(FLOAT, FLOAT, FLOAT, FLOAT, FLOAT, FLOAT, FLOAT, DIRECTION);

	void LoadTextures() override;
	void LoadSprites() override;
	void LoadAnimations() override;

	const Bill* GetEnemyTarget();

protected:
	RifleManHideOnBushState* state;
	RifleManHideOnBushState* updateState;

};

class RifleManHideOnBushState : public State<RifleManHideOnBushState, RifleManHideOnBush>
{
public: 
	RifleManHideOnBushState();
	~RifleManHideOnBushState();

	virtual void Exit(RifleManHideOnBush&) = 0;
	virtual void Enter(RifleManHideOnBush&) = 0;
	virtual void Render(RifleManHideOnBush&) = 0;

	virtual RifleManHideOnBushState* Update(RifleManHideOnBush&) = 0;
	virtual RifleManHideOnBushState* HandleInput(RifleManHideOnBush&, Input&) override;

protected:
	FLOAT time;
};

class RifleManHideOnBushHidingState : public RifleManHideOnBushState
{
public:
	RifleManHideOnBushHidingState();
	~RifleManHideOnBushHidingState();

	virtual void Exit(RifleManHideOnBush&);
	virtual void Enter(RifleManHideOnBush&);
	virtual void Render(RifleManHideOnBush&);

	virtual RifleManHideOnBushState* Update(RifleManHideOnBush&);
};

class RifleManHideOnBushAppearState : public RifleManHideOnBushState
{
public:
	RifleManHideOnBushAppearState();
	~RifleManHideOnBushAppearState();

	virtual void Exit(RifleManHideOnBush&);
	virtual void Enter(RifleManHideOnBush&);
	virtual void Render(RifleManHideOnBush&);

	virtual RifleManHideOnBushState* Update(RifleManHideOnBush&);
};

class RifleManHideOnBushStandingState : public RifleManHideOnBushState
{
public:
	RifleManHideOnBushStandingState(RifleManHideOnBush&);
	~RifleManHideOnBushStandingState();

	virtual void Exit(RifleManHideOnBush&);
	virtual void Enter(RifleManHideOnBush&);
	virtual void Render(RifleManHideOnBush&);

	virtual RifleManHideOnBushState* Update(RifleManHideOnBush&);
};

class RifleManHideOnBushHideState : public RifleManHideOnBushState
{
public:
	RifleManHideOnBushHideState();
	~RifleManHideOnBushHideState();

	virtual void Exit(RifleManHideOnBush&);
	virtual void Enter(RifleManHideOnBush&);
	virtual void Render(RifleManHideOnBush&);

	virtual RifleManHideOnBushState* Update(RifleManHideOnBush&);
};