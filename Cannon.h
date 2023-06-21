#pragma once
#include "State.h"
#include "Common.h"
#include "Entity.h"
#include "HasAnimations.h"
#include "HasSprites.h"
#include "HasTextures.h"
#include "Bill.h"
#include "Enemy.h"
#include "HasWeapons.h"

#define SHOOT_DELAY 60
#define SHOOT_DELAY_PER_BULLET 20
#define SHOOT_TIME 3

class Cannon;
class CannonState;
class CannonAppearState;
class CannonNormalState;
class CannonUp30State;
class CannonUp60State;

class Cannon : public Entity, public Enemy<Bill>
	, public HasTextures<Cannon>, public HasSprites<Cannon>, public HasAnimations<Cannon>, public HasWeapons
{
public:
	Cannon();
	~Cannon();

	virtual void Update() override;
	virtual void Render() override;
	virtual void HandleInput(Input&) override;

	void LoadTextures() override;
	void LoadSprites() override;
	void LoadAnimations() override;

	void Fire() override;
	void Fire(FLOAT, FLOAT, FLOAT, FLOAT, FLOAT, DIRECTION);

	const Bill* GetEnemyTarget();

	FLOAT CalculateShootingAngle();
	BOOLEAN IsTargetInRange();

	int shootDelay;
	int shootDelayPerBullet;
	int shootTime;

protected:
	CannonState* state;
	CannonState* updateState;
};

class CannonState : public State<CannonState, Cannon>
{
public:
	CannonState();
	~CannonState();

	virtual void Exit(Cannon&) = 0;
	virtual void Enter(Cannon&) = 0;
	virtual void Render(Cannon&) = 0;

	virtual CannonState* Update(Cannon&) = 0;
	virtual CannonState* HandleInput(Cannon&, Input&) override;

protected:
	FLOAT time;
};

class CannonAppearState : public CannonState
{
public:
	CannonAppearState();
	~CannonAppearState();

	virtual void Exit(Cannon&);
	virtual void Enter(Cannon&);
	virtual void Render(Cannon&);

	virtual CannonState* Update(Cannon&);
};

class CannonNormalState: public CannonState
{
public:
	CannonNormalState();
	~CannonNormalState();

	virtual void Exit(Cannon&);
	virtual void Enter(Cannon&);
	virtual void Render(Cannon&);

	virtual CannonState* Update(Cannon&);
};

class CannonUp30State : public CannonState
{
public:
	CannonUp30State();
	~CannonUp30State();

	virtual void Exit(Cannon&);
	virtual void Enter(Cannon&);
	virtual void Render(Cannon&);

	virtual CannonState* Update(Cannon&);
};

class CannonUp60State : public CannonState
{
public:
	CannonUp60State();
	~CannonUp60State();

	virtual void Exit(Cannon&);
	virtual void Enter(Cannon&);
	virtual void Render(Cannon&);

	virtual CannonState* Update(Cannon&);
};