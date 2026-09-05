#pragma once
#include "Enemy.h"
#include "State.h"
#include "Common.h"
#include "Entity.h"
#include "HasSprites.h"
#include "HasTextures.h"
#include "HasAnimations.h"
#include "Bill.h"

class ScubaSoldierState;
class ScubaSoldierHiddenState;
class ScubaSoldierShootingState;

class ScubaSoldier : public Entity, public Enemy<Bill>
				   , public HasTextures<ScubaSoldier>, public HasSprites<ScubaSoldier>, public HasAnimations<ScubaSoldier>, public HasWeapons
{
public:

	ScubaSoldier();
	virtual ~ScubaSoldier();
	void Update() override;
	void Render() override;
	void HandleInput(Input& input) override;

	void LoadSprites() override;
	void LoadTextures() override;
	void LoadAnimations() override;

	void Fire() override;
	void CalculateBillAngle();

	bool IsEnemy() const override { return true; }
	ENEMY_TYPE GetEnemyType() const override { return this->_enemyType; }
	bool IsLethalToTouch() const override { return true; }
	void SetTarget(const Bill* target) override { this->Enemy<Bill>::SetTarget(target); }
	bool TakeBulletHit() override { return this->Enemy<Bill>::TakeEnemyBulletHit(this); }
protected:

	ScubaSoldierState* _state;
	ScubaSoldierState* _updateState;
	ScubaSoldierState* _handleInputState;

	float _billAngle;
};

// build state of soldier
class ScubaSoldierState : public State<ScubaSoldierState, ScubaSoldier>
{

public:

	ScubaSoldierState();
	virtual ~ScubaSoldierState();

	virtual void Exit(ScubaSoldier& scubaSoldier) override = 0;
	virtual void Enter(ScubaSoldier& scubaSoldier) override = 0;
	virtual void Render(ScubaSoldier& scubaSoldier) override = 0;

	virtual ScubaSoldierState* Update(ScubaSoldier& scubaSoldier) override = 0;
	virtual ScubaSoldierState* HandleInput(ScubaSoldier& scubaSoldier, Input& input) override = 0;

	virtual bool IsHidden() const { return false; }

protected:

	float _time;

};

// shooting state
class ScubaSoldierShootingState : public ScubaSoldierState
{

public:

	ScubaSoldierShootingState();
	virtual ~ScubaSoldierShootingState();

	virtual void Exit(ScubaSoldier& scubaSoldier) override;
	virtual void Enter(ScubaSoldier& scubaSoldier) override;
	virtual void Render(ScubaSoldier& scubaSoldier) override;

	virtual ScubaSoldierState* Update(ScubaSoldier& scubaSoldier) override;
	virtual ScubaSoldierState* HandleInput(ScubaSoldier& scubaSoldier, Input& input) override;

};

// hidden state
class ScubaSoldierHiddenState : public ScubaSoldierState
{

public:

	ScubaSoldierHiddenState();
	virtual ~ScubaSoldierHiddenState();

	virtual void Exit(ScubaSoldier& scubaSoldier) override;
	virtual void Enter(ScubaSoldier& scubaSoldier) override;
	virtual void Render(ScubaSoldier& scubaSoldier) override;

	virtual ScubaSoldierState* Update(ScubaSoldier& scubaSoldier) override;
	virtual ScubaSoldierState* HandleInput(ScubaSoldier& scubaSoldier, Input& input) override;

	bool IsHidden() const override { return true; }

};
