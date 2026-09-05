#pragma once
#include "State.h"
#include "Common.h"
#include "Entity.h"
#include "HasAnimations.h"
#include "HasSprites.h"
#include "HasTextures.h"
#include "Bill.h"
#include "Enemy.h"

class RockFall;
class RockFallState;
class RockFallNormalState;
class RockFallFallState;


class RockFall : public Entity, public Enemy<Bill>
	, public HasTextures<RockFall>, public HasSprites<RockFall>, public HasAnimations<RockFall>, public CollidableEntity
{
public:
	RockFall();
	virtual ~RockFall();
	void Update() override;
	void Render() override;
	void HandleInput(Input& input) override;

	void LoadSprites() override;
	void LoadTextures() override;
	void LoadAnimations() override;

	void SetTimeDelayToFall(float timeDelayToFall)
	{
		this->_timeDelayToFall = timeDelayToFall;
	}
	float GetTimeDelayToFall() const
	{
		return this->_timeDelayToFall;
	}

	bool HasBouncedBack() const { return this->_bouncedBack; }
	void SetBouncedBack(bool bouncedBack) { this->_bouncedBack = bouncedBack; }

	CollidableEntity* AsCollidable() override { return this; }
	void  StaticResolveNoCollision() override;
	void  StaticResolveOnCollision(AABBSweepResult aabbSweepResult) override;
	void DynamicResolveNoCollision() override;
	void DynamicResolveOnCollision(AABBSweepResult aabbSweepResult) override;

	bool IsEnemy() const override { return true; }
	bool IsLethalToTouch() const override { return true; }
	ENEMY_TYPE GetEnemyType() const override { return this->_enemyType; }
	bool TakeBulletHit() override { return this->Enemy<Bill>::TakeEnemyBulletHit(this); }
	void SetTarget(const Bill* target) override { this->Enemy<Bill>::SetTarget(target); }
protected:
	std::unordered_set<Entity*> _alreadyCollidedWithEntities;
	bool _bouncedBack = false;

	RockFallState* _state;
	RockFallState* _updateState;
	RockFallState* _handleInputState;

	float _timeDelayToFall;
};



class RockFallState : public State<RockFallState, RockFall>
{

public:

	RockFallState();
	virtual ~RockFallState();

	virtual void Exit(RockFall& rockFall) override = 0;
	virtual void Enter(RockFall& rockFall) override = 0;
	virtual void Render(RockFall& rockFall) override = 0;

	virtual RockFallState* Update(RockFall& rockFall) override = 0;
	virtual RockFallState* HandleInput(RockFall& rockFall, Input& input) override = 0;

protected:

	float _time;

};


class RockFallNormalState : public RockFallState
{

public:

	RockFallNormalState();
	virtual ~RockFallNormalState();

	virtual void Exit(RockFall& rockFall) override;
	virtual void Enter(RockFall& rockFall) override;
	virtual void Render(RockFall& rockFall) override;

	virtual RockFallState* Update(RockFall& rockFall) override;
	virtual RockFallState* HandleInput(RockFall& rockFall, Input& input) override;
};


class RockFallFallState : public RockFallState
{

public:

	RockFallFallState();
	virtual ~RockFallFallState();

	virtual void Exit(RockFall& rockFall) override;
	virtual void Enter(RockFall& rockFall) override;
	virtual void Render(RockFall& rockFall) override;

	virtual RockFallState* Update(RockFall& rockFall) override;
	virtual RockFallState* HandleInput(RockFall& rockFall, Input& input) override;
};
