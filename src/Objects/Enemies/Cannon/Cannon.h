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
	virtual void HandleInput(Input& input) override;

	void LoadTextures() override;
	void LoadSprites() override;
	void LoadAnimations() override;

	void Fire() override;
	void Fire(float angle, float vx, float vy, float ax, float ay, DIRECTION direction);

	const Bill* GetEnemyTarget();

	float CalculateShootingAngle() const;
	bool IsTargetInRange() const;

	// Burst pacing, ticked once per logic step by CannonState::UpdateShooting:
	// _shootDelay gates the burst as a whole, _shootDelayPerBullet paces the
	// rounds inside it, and _shootTime counts the rounds still owed.
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

	CannonState* _state;
	CannonState* _updateState;
};

class CannonState : public State<CannonState, Cannon>
{
public:
	CannonState();
	~CannonState();

	virtual void Exit(Cannon& cannon) = 0;
	virtual void Enter(Cannon& cannon) = 0;
	virtual void Render(Cannon& cannon) = 0;

	virtual CannonState* Update(Cannon& cannon) = 0;
	virtual CannonState* HandleInput(Cannon& cannon, Input& input) override;

protected:
	float _time;
	bool UpdateShooting(Cannon& cannon);
};

class CannonAppearState : public CannonState
{
public:
	CannonAppearState();
	~CannonAppearState();

	virtual void Exit(Cannon& cannon);
	virtual void Enter(Cannon& cannon);
	virtual void Render(Cannon& cannon);

	virtual CannonState* Update(Cannon& cannon);
};

class CannonNormalState: public CannonState
{
public:
	CannonNormalState();
	~CannonNormalState();

	virtual void Exit(Cannon& cannon);
	virtual void Enter(Cannon& cannon);
	virtual void Render(Cannon& cannon);

	virtual CannonState* Update(Cannon& cannon);
};

class CannonUp30State : public CannonState
{
public:
	CannonUp30State();
	~CannonUp30State();

	virtual void Exit(Cannon& cannon);
	virtual void Enter(Cannon& cannon);
	virtual void Render(Cannon& cannon);

	virtual CannonState* Update(Cannon& cannon);
};

class CannonUp60State : public CannonState
{
public:
	CannonUp60State();
	~CannonUp60State();

	virtual void Exit(Cannon& cannon);
	virtual void Enter(Cannon& cannon);
	virtual void Render(Cannon& cannon);

	virtual CannonState* Update(Cannon& cannon);
};
