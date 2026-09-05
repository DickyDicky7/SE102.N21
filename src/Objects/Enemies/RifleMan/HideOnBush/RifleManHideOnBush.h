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
	virtual void HandleInput(Input& input) override;

	void Fire() override;
	void CustomFire(float x, float y, float angle, float vx, float vy, float ax, float ay, DIRECTION movingDirection);

	void LoadTextures() override;
	void LoadSprites() override;
	void LoadAnimations() override;

	const Bill* GetEnemyTarget();

	bool IsEnemy() const override { return true; }
	ENEMY_TYPE GetEnemyType() const override { return this->_enemyType; }
	bool IsLethalToTouch() const override { return true; }
	void SetTarget(const Bill* target) override { this->Enemy<Bill>::SetTarget(target); }
	bool TakeBulletHit() override { return this->Enemy<Bill>::TakeEnemyBulletHit(this); }
protected:
	RifleManHideOnBushState* _state;
	RifleManHideOnBushState* _updateState;

};

class RifleManHideOnBushState : public State<RifleManHideOnBushState, RifleManHideOnBush>
{
public:
	RifleManHideOnBushState();
	~RifleManHideOnBushState();

	virtual void Exit(RifleManHideOnBush& rifleManHideOnBush) = 0;
	virtual void Enter(RifleManHideOnBush& rifleManHideOnBush) = 0;
	virtual void Render(RifleManHideOnBush& rifleManHideOnBush) = 0;

	virtual RifleManHideOnBushState* Update(RifleManHideOnBush& rifleManHideOnBush) = 0;
	virtual RifleManHideOnBushState* HandleInput(RifleManHideOnBush& rifleManHideOnBush, Input& input) override;

protected:
	float _time;
};

class RifleManHideOnBushHidingState : public RifleManHideOnBushState
{
public:
	RifleManHideOnBushHidingState();
	~RifleManHideOnBushHidingState();

	virtual void Exit(RifleManHideOnBush& rifleManHideOnBush);
	virtual void Enter(RifleManHideOnBush& rifleManHideOnBush);
	virtual void Render(RifleManHideOnBush& rifleManHideOnBush);

	virtual RifleManHideOnBushState* Update(RifleManHideOnBush& rifleManHideOnBush);
};

class RifleManHideOnBushAppearState : public RifleManHideOnBushState
{
public:
	RifleManHideOnBushAppearState();
	~RifleManHideOnBushAppearState();

	virtual void Exit(RifleManHideOnBush& rifleManHideOnBush);
	virtual void Enter(RifleManHideOnBush& rifleManHideOnBush);
	virtual void Render(RifleManHideOnBush& rifleManHideOnBush);

	virtual RifleManHideOnBushState* Update(RifleManHideOnBush& rifleManHideOnBush);
};

class RifleManHideOnBushStandingState : public RifleManHideOnBushState
{
public:
	RifleManHideOnBushStandingState(RifleManHideOnBush& rifleManHideOnBush);
	~RifleManHideOnBushStandingState();

	virtual void Exit(RifleManHideOnBush& rifleManHideOnBush);
	virtual void Enter(RifleManHideOnBush& rifleManHideOnBush);
	virtual void Render(RifleManHideOnBush& rifleManHideOnBush);

	virtual RifleManHideOnBushState* Update(RifleManHideOnBush& rifleManHideOnBush);
protected:
	bool _isShoot;
};

class RifleManHideOnBushHideState : public RifleManHideOnBushState
{
public:
	RifleManHideOnBushHideState();
	~RifleManHideOnBushHideState();

	virtual void Exit(RifleManHideOnBush& rifleManHideOnBush);
	virtual void Enter(RifleManHideOnBush& rifleManHideOnBush);
	virtual void Render(RifleManHideOnBush& rifleManHideOnBush);

	virtual RifleManHideOnBushState* Update(RifleManHideOnBush& rifleManHideOnBush);
};
