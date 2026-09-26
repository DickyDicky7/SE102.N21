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
	virtual void HandleInput(Input& input) override;

	void Fire() override;
	void CustomFire(float x, float y, float angle, float vx, float vy, float ax, float ay, DIRECTION movingDirection);

	void LoadTextures() override;
	void LoadSprites() override;
	void LoadAnimations() override;

	const Bill* GetEnemyTarget();

	float CalculateShootingAngle() const;

	// Burst pacing, ticked once per logic step by
	// RifleManStandingState::UpdateShooting: _shootDelay gates the burst as a
	// whole, _shootDelayPerBullet paces the rounds inside it, and _shootTime
	// counts the rounds still owed.
	int  TickShootDelay();
	int  TickShootDelayPerBullet();
	int  GetShotsLeftInBurst() const;
	void ConsumeShotInBurst();
	void ResetShootDelayPerBullet();
	void ResetBurst();

	bool IsEnemy() const override;
	ENEMY_TYPE GetEnemyType() const override;
	bool IsLethalToTouch() const override;
	void SetTarget(const Bill* target) override;
	bool TakeBulletHit() override;
protected:
	int _shootDelay;
	int _shootDelayPerBullet;
	int _shootTime;

	RifleManStandingState* _state;
	RifleManStandingState* _updateState;
};

class RifleManStandingState : public State<RifleManStandingState, RifleManStanding>
{
public:
	RifleManStandingState();
	~RifleManStandingState();

	virtual void Exit(RifleManStanding& rifleManStanding) = 0;
	virtual void Enter(RifleManStanding& rifleManStanding) = 0;
	virtual void Render(RifleManStanding& rifleManStanding) = 0;

	virtual RifleManStandingState* Update(RifleManStanding& rifleManStanding) = 0;
	virtual RifleManStandingState* HandleInput(RifleManStanding& rifleManStanding, Input& input) override;

protected:
	float _time;
	bool UpdateShooting(RifleManStanding& rifleManStanding);
};

class RifleManStandingNormalState : public RifleManStandingState
{
public:
	RifleManStandingNormalState();
	~RifleManStandingNormalState();

	virtual void Exit(RifleManStanding& rifleManStanding);
	virtual void Enter(RifleManStanding& rifleManStanding);
	virtual void Render(RifleManStanding& rifleManStanding);

	virtual RifleManStandingState* Update(RifleManStanding& rifleManStanding);
};

class RifleManStandingAimUpState : public RifleManStandingState
{
public:
	RifleManStandingAimUpState();
	~RifleManStandingAimUpState();

	virtual void Exit(RifleManStanding& rifleManStanding);
	virtual void Enter(RifleManStanding& rifleManStanding);
	virtual void Render(RifleManStanding& rifleManStanding);

	virtual RifleManStandingState* Update(RifleManStanding& rifleManStanding);
};

class RifleManStandingAimDownState : public RifleManStandingState
{
public:
	RifleManStandingAimDownState();
	~RifleManStandingAimDownState();

	virtual void Exit(RifleManStanding& rifleManStanding);
	virtual void Enter(RifleManStanding& rifleManStanding);
	virtual void Render(RifleManStanding& rifleManStanding);

	virtual RifleManStandingState* Update(RifleManStanding& rifleManStanding);
};
