#pragma once
#include "Bill.h"
#include "Enemy.h"
#include "State.h"
#include "Common.h"
#include "Entity.h"
#include "HasSprites.h"
#include "HasTextures.h"
#include "HasAnimations.h"

class SoldierState;
class SoldierRunState;
class SoldierJumpState;
class SoldierShootState;
class SoldierLayDownState;
class SoldierDieState;

class Soldier : public Entity, public Enemy<Bill>
		      , public HasTextures<Soldier>, public HasSprites<Soldier>, public HasAnimations<Soldier>, public CollidableEntity, public HasWeapons
{
public:

	Soldier();
	virtual ~Soldier();
	void Update() override;
	void Render() override;
	void HandleInput(Input& input) override;

	void LoadSprites() override;
	void LoadTextures() override;
	void LoadAnimations() override;

	SoldierState* GetState();
	void SetState(SoldierState* state);
	void GoDead();
	bool IsShootable() const { return this->_shootable; }
	void SetShootable(bool shootable) { this->_shootable = shootable; }
	void Fire() override;
	CollidableEntity* AsCollidable() override { return this; }
	void StaticResolveNoCollision() override;
	void StaticResolveOnCollision(AABBSweepResult aabbSweepResult) override;
	void DynamicResolveNoCollision() override;
	void DynamicResolveOnCollision(AABBSweepResult aabbSweepResult) override;

	bool IsEnemy() const override { return true; }
	ENEMY_TYPE GetEnemyType() const override { return this->_enemyType; }
	bool CollidesWithBoundaryWalls() const override { return true; }
	bool IsLethalToTouch() const override;
	void SetTarget(const Bill* target) override { this->Enemy<Bill>::SetTarget(target); }
	bool TakeBulletHit() override;
protected:

	bool _shootable;
	SoldierState* _state;
	SoldierState* _updateState;
	SoldierState* _handleInputState;
};

// build state of soldier
class SoldierState : public State<SoldierState, Soldier>
{

public:

	SoldierState();
	virtual ~SoldierState();

	virtual void Exit(Soldier& soldier) override = 0;
	virtual void Enter(Soldier& soldier) override = 0;
	virtual void Render(Soldier& soldier) override = 0;

	virtual SoldierState* Update(Soldier& soldier) override = 0;
	virtual SoldierState* HandleInput(Soldier& soldier, Input& input) override = 0;

	virtual float GetGunMountOffsetRatio() const { return 0.0f; }
	virtual bool IsDead() const { return false; }
	virtual bool IsJumping() const { return false; }
	virtual bool IsShooting() const { return false; }
	virtual bool IsLayingDown() const { return false; }

protected:

	float _time;

};

class SoldierRunState : public SoldierState
{

public:

	SoldierRunState();
	virtual ~SoldierRunState();

	virtual void Exit(Soldier& soldier) override;
	virtual void Enter(Soldier& soldier) override;
	virtual void Render(Soldier& soldier) override;

	virtual SoldierState* Update(Soldier& soldier) override;
	virtual SoldierState* HandleInput(Soldier& soldier, Input& input) override;

};

class SoldierJumpState : public SoldierState
{

public:

	SoldierJumpState();
	virtual ~SoldierJumpState();

	virtual void Exit(Soldier& soldier) override;
	virtual void Enter(Soldier& soldier) override;
	virtual void Render(Soldier& soldier) override;

	virtual SoldierState* Update(Soldier& soldier) override;
	virtual SoldierState* HandleInput(Soldier& soldier, Input& input) override;

	bool IsJumping() const override { return true; }

protected:

	bool _hasMovedLeft;
	bool _hasMovedRight;

};

class SoldierShootState : public SoldierState
{

public:

	SoldierShootState();
	virtual ~SoldierShootState();

	virtual void Exit(Soldier& soldier) override;
	virtual void Enter(Soldier& soldier) override;
	virtual void Render(Soldier& soldier) override;

	virtual SoldierState* Update(Soldier& soldier) override;
	virtual SoldierState* HandleInput(Soldier& soldier, Input& input) override;

	float GetGunMountOffsetRatio() const override { return Constants::Enemies::Soldier::GUN_MOUNT_OFFSET_RATIO_STANDING; }
	bool IsShooting() const override { return true; }

};

class SoldierLayDownState : public SoldierState
{

public:

	SoldierLayDownState();
	virtual ~SoldierLayDownState();

	virtual void Exit(Soldier& soldier) override;
	virtual void Enter(Soldier& soldier) override;
	virtual void Render(Soldier& soldier) override;

	virtual SoldierState* Update(Soldier& soldier) override;
	virtual SoldierState* HandleInput(Soldier& soldier, Input& input) override;

	float GetGunMountOffsetRatio() const override { return Constants::Enemies::Soldier::GUN_MOUNT_OFFSET_RATIO_PRONE; }
	bool IsLayingDown() const override { return true; }

};

class SoldierDieState : public SoldierState
{

public:

	SoldierDieState();
	virtual ~SoldierDieState();

	virtual void Exit(Soldier& soldier) override;
	virtual void Enter(Soldier& soldier) override;
	virtual void Render(Soldier& soldier) override;

	virtual SoldierState* Update(Soldier& soldier) override;
	virtual SoldierState* HandleInput(Soldier& soldier, Input& input) override;

	bool IsDead() const override { return true; }

};
