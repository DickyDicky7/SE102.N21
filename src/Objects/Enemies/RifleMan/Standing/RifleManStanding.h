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

#define RILFE_MAN_STANDING_SHOOT_DELAY 120
#define RILFE_MAN_STANDING_SHOOT_DELAY_PER_BULLET 20
#define RILFE_MAN_STANDING_SHOOT_TIME 3

class RifleManStanding;
class RifleManStandingState;
class RifleManStandingNormalState;
class RifleManStandingAimUpState;
class RifleManStandingAimDownState;

class RifleManStanding : public Entity, public Enemy<Bill>
	, public HasTextures<RifleManStanding>, public HasSprites<RifleManStanding>, public HasAnimations<RifleManStanding>, public RifleMan, public HasWeapons
{
public:
	RifleManStanding();
	~RifleManStanding();

	virtual void Update() override;
	virtual void Render() override;
	virtual void HandleInput(Input&) override;

	void Fire() override;
	void CustomFire(FLOAT, FLOAT, FLOAT, FLOAT, FLOAT, FLOAT, FLOAT, DIRECTION);

	void LoadTextures() override;
	void LoadSprites() override;
	void LoadAnimations() override;

	const Bill* GetEnemyTarget();

	FLOAT CalculateShootingAngle();

	int shootDelay;
	int shootDelayPerBullet;
	int shootTime;

protected:
	RifleManStandingState* state;
	RifleManStandingState* updateState;
};

class RifleManStandingState : public State<RifleManStandingState, RifleManStanding>
{
public:
	RifleManStandingState();
	~RifleManStandingState();

	virtual void Exit(RifleManStanding&) = 0;
	virtual void Enter(RifleManStanding&) = 0;
	virtual void Render(RifleManStanding&) = 0;

	virtual RifleManStandingState* Update(RifleManStanding&) = 0;
	virtual RifleManStandingState* HandleInput(RifleManStanding&, Input&) override;

protected:
	FLOAT time;
};

class RifleManStandingNormalState : public RifleManStandingState
{
public:
	RifleManStandingNormalState();
	~RifleManStandingNormalState();

	virtual void Exit(RifleManStanding&);
	virtual void Enter(RifleManStanding&);
	virtual void Render(RifleManStanding&);

	virtual RifleManStandingState* Update(RifleManStanding&);
};

class RifleManStandingAimUpState : public RifleManStandingState
{
public:
	RifleManStandingAimUpState();
	~RifleManStandingAimUpState();

	virtual void Exit(RifleManStanding&);
	virtual void Enter(RifleManStanding&);
	virtual void Render(RifleManStanding&);

	virtual RifleManStandingState* Update(RifleManStanding&);
};

class RifleManStandingAimDownState : public RifleManStandingState
{
public:
	RifleManStandingAimDownState();
	~RifleManStandingAimDownState();

	virtual void Exit(RifleManStanding&);
	virtual void Enter(RifleManStanding&);
	virtual void Render(RifleManStanding&);

	virtual RifleManStandingState* Update(RifleManStanding&);
};