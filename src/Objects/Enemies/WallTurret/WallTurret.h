#pragma once
#include "State.h"
#include "Common.h"
#include "Entity.h"
#include "HasAnimations.h"
#include "HasSprites.h"
#include "HasTextures.h"
#include "HasWeapons.h"
#include "Bill.h"
#include "Enemy.h"


class WallTurret;
class WallTurretState;

class WallTurretNormalState;

class WallTurretOpeningState;
class WallTurretClosingState;

class WallTurretUpState;
class WallTurretDownState;

//angle right side of circle
class WallTurretRight30State;
class WallTurretRight60State;
class WallTurretRight90State;
class WallTurretRight120State;
class WallTurretRight180State;


//angle left side of circle
class WallTurretLeft30State;
class WallTurretLeft60State;
class WallTurretLeft90State;
class WallTurretLeft120State;
class WallTurretLeft150State;


class WallTurret : public Entity, public Enemy<Bill>
	, public HasTextures<WallTurret>, public HasSprites<WallTurret>, public HasAnimations<WallTurret>, public HasWeapons
{
public:
	WallTurret();
	virtual ~WallTurret();
	void Update() override;
	void Render() override;
	void HandleInput(Input& input) override;

	void LoadTextures() override;
	void LoadSprites() override;
	void LoadAnimations() override;

	void Fire() override;
	void Fire(float angle, float vx, float vy, float ax, float ay, DIRECTION direction);

	float CalculateBillAngle();

	bool IsTargetInRange() const;

	// Countdown to the next shot, ticked once per logic step by
	// WallTurretState::UpdateShooting.
	int  TickShootDelay()  { return --this->_shootDelay; }
	void ResetShootDelay() { this->_shootDelay = Constants::Enemies::WallTurret::SHOOT_DELAY_FRAMES; }

	bool IsEnemy() const override { return true; }
	ENEMY_TYPE GetEnemyType() const override { return this->_enemyType; }
	bool IsLethalToTouch() const override { return true; }
	void SetTarget(const Bill* target) override { this->Enemy<Bill>::SetTarget(target); }
	bool TakeBulletHit() override { return this->Enemy<Bill>::TakeEnemyBulletHit(this); }
protected:
	int _shootDelay;

	WallTurretState* _state;
	WallTurretState* _updateState;
};

class WallTurretState : public State<WallTurretState, WallTurret>
{
public:
	WallTurretState();
	virtual ~WallTurretState();

	virtual void Exit(WallTurret& wallTurret) override;
	virtual void Enter(WallTurret& wallTurret) override;
	virtual void Render(WallTurret& wallTurret) override;

	virtual WallTurretState* Update(WallTurret& wallTurret) override;
	virtual WallTurretState* HandleInput(WallTurret& wallTurret, Input& input) override;

protected:
	int _delayBeforeChangeState;
	bool UpdateShooting(WallTurret& wallTurret);
};

class WallTurretLeft90State : public WallTurretState
{
public:
	WallTurretLeft90State();
	virtual ~WallTurretLeft90State();

	virtual void Exit(WallTurret& wallTurret) override;
	virtual void Enter(WallTurret& wallTurret) override;
	virtual void Render(WallTurret& wallTurret) override;

	virtual WallTurretState* Update(WallTurret& wallTurret) override;
};

class WallTurretRight90State : public WallTurretState
{
public:
	WallTurretRight90State();
	virtual ~WallTurretRight90State();

	virtual void Exit(WallTurret& wallTurret) override;
	virtual void Enter(WallTurret& wallTurret) override;
	virtual void Render(WallTurret& wallTurret) override;

	virtual WallTurretState* Update(WallTurret& wallTurret) override;
};

class WallTurretUpState : public WallTurretState
{
public:
	WallTurretUpState();
	virtual ~WallTurretUpState();

	virtual void Render(WallTurret& wallTurret) override;
	virtual void Exit(WallTurret& wallTurret) override;
	virtual void Enter(WallTurret& wallTurret) override;

	virtual WallTurretState* Update(WallTurret& wallTurret) override;
};

class WallTurretDownState : public WallTurretState
{
public:
	WallTurretDownState();
	virtual ~WallTurretDownState();

	virtual void Render(WallTurret& wallTurret) override;
	virtual void Exit(WallTurret& wallTurret) override;
	virtual void Enter(WallTurret& wallTurret) override;

	virtual WallTurretState* Update(WallTurret& wallTurret) override;
};

class WallTurretRight60State : public WallTurretState
{
public:
	WallTurretRight60State();
	virtual ~WallTurretRight60State();

	virtual void Render(WallTurret& wallTurret) override;
	virtual void Exit(WallTurret& wallTurret) override;
	virtual void Enter(WallTurret& wallTurret) override;

	virtual WallTurretState* Update(WallTurret& wallTurret) override;
};

class WallTurretRight30State : public WallTurretState
{
public:
	WallTurretRight30State();
	virtual ~WallTurretRight30State();

	virtual void Render(WallTurret& wallTurret) override;
	virtual void Exit(WallTurret& wallTurret) override;
	virtual void Enter(WallTurret& wallTurret) override;

	virtual WallTurretState* Update(WallTurret& wallTurret) override;
};

class WallTurretRight120State : public WallTurretState
{
public:
	WallTurretRight120State();
	virtual ~WallTurretRight120State();

	virtual void Render(WallTurret& wallTurret) override;
	virtual void Exit(WallTurret& wallTurret) override;
	virtual void Enter(WallTurret& wallTurret) override;

	virtual WallTurretState* Update(WallTurret& wallTurret) override;
};

class WallTurretRight150State : public WallTurretState
{
public:
	WallTurretRight150State();
	virtual ~WallTurretRight150State();

	virtual void Render(WallTurret& wallTurret) override;
	virtual void Exit(WallTurret& wallTurret) override;
	virtual void Enter(WallTurret& wallTurret) override;

	virtual WallTurretState* Update(WallTurret& wallTurret) override;
};

class WallTurretLeft60State : public WallTurretState
{
public:
	WallTurretLeft60State();
	virtual ~WallTurretLeft60State();

	virtual void Render(WallTurret& wallTurret) override;
	virtual void Exit(WallTurret& wallTurret) override;
	virtual void Enter(WallTurret& wallTurret) override;

	virtual WallTurretState* Update(WallTurret& wallTurret) override;
};

class WallTurretLeft30State : public WallTurretState
{
public:
	WallTurretLeft30State();
	virtual ~WallTurretLeft30State();

	virtual void Render(WallTurret& wallTurret) override;
	virtual void Exit(WallTurret& wallTurret) override;
	virtual void Enter(WallTurret& wallTurret) override;

	virtual WallTurretState* Update(WallTurret& wallTurret) override;
};

class WallTurretLeft120State : public WallTurretState
{
public:
	WallTurretLeft120State();
	virtual ~WallTurretLeft120State();

	virtual void Render(WallTurret& wallTurret) override;
	virtual void Exit(WallTurret& wallTurret) override;
	virtual void Enter(WallTurret& wallTurret) override;

	virtual WallTurretState* Update(WallTurret& wallTurret) override;
};

class WallTurretLeft150State : public WallTurretState
{
public:
	WallTurretLeft150State();
	virtual ~WallTurretLeft150State();

	virtual void Render(WallTurret& wallTurret) override;
	virtual void Exit(WallTurret& wallTurret) override;
	virtual void Enter(WallTurret& wallTurret) override;

	virtual WallTurretState* Update(WallTurret& wallTurret) override;
};

class WallTurretOpeningState : public WallTurretState
{
public:
	WallTurretOpeningState();
	virtual ~WallTurretOpeningState();

	virtual void Render(WallTurret& wallTurret) override;
	virtual void Exit(WallTurret& wallTurret) override;
	virtual void Enter(WallTurret& wallTurret) override;

	virtual WallTurretState* Update(WallTurret& wallTurret) override;
};

class WallTurretClosingState : public WallTurretState
{
public:
	WallTurretClosingState();
	virtual ~WallTurretClosingState();

	virtual void Render(WallTurret& wallTurret) override;
	virtual void Exit(WallTurret& wallTurret) override;
	virtual void Enter(WallTurret& wallTurret) override;

	virtual WallTurretState* Update(WallTurret& wallTurret) override;
};

class WallTurretNormalState : public WallTurretState
{
public:
	WallTurretNormalState();
	virtual ~WallTurretNormalState();

	virtual void Render(WallTurret& wallTurret) override;
	virtual void Exit(WallTurret& wallTurret) override;
	virtual void Enter(WallTurret& wallTurret) override;

	virtual WallTurretState* Update(WallTurret& wallTurret) override;
};
