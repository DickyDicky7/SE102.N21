﻿#pragma once


#include "Sound.h"
#include "State.h"
#include "Motion.h"
#include "Common.h"
#include "Entity.h"
#include "HasSprites.h"
#include "HasTextures.h"
#include "HasAnimations.h"
#include "CollidableEntity.h"


class Bullet;
class BulletState;
class BulletRState;
class BulletMState;
class BulletSState;
class BulletLState;
class BulletFState;
class BulletEnemyState;
class BulletExplodeState;
class BulletBossStage1State;
class BulletScubaSoldierState;
class BulletBossStage2StateHand;
class BulletBossStage2StateHead;


class Bullet : public Entity, public HasTextures<Bullet>, public HasSprites<Bullet>, public HasAnimations<Bullet>, public CollidableEntity
{

public:

	BOOL isFake ;
	BOOL isEnemy;

	Bullet(            );
	Bullet(BulletState*);
	virtual ~Bullet();
	void Update() override;
	void Render() override;
	void HandleInput(Input&) override;

	void SetState(BulletState*);
	BulletState* GetState() const;
	void LoadSprites() override;
	void LoadTextures() override;
	void LoadAnimations() override;

	void  StaticResolveNoCollision(               ) override;
	void  StaticResolveOnCollision(AABBSweepResult) override;
	void DynamicResolveNoCollision(               ) override;
	void DynamicResolveOnCollision(AABBSweepResult) override;

protected:

	BulletState* state;
	BulletState* updateState;
	BulletState* handleInputState;

};


class BulletState : public State<BulletState, Bullet>
{

public:

	BulletState();
	virtual ~BulletState();

	virtual void Exit(Bullet&) override = 0;
	virtual void Enter(Bullet&) override = 0;
	virtual void Render(Bullet&) override = 0;

	virtual BulletState* Update(Bullet&) override = 0;
	virtual BulletState* HandleInput(Bullet&, Input&) override = 0;

protected:

	FLOAT time;

};


class BulletRState : public BulletState
{

public:

	BulletRState();
	virtual ~BulletRState();

	virtual void Exit(Bullet&) override;
	virtual void Enter(Bullet&) override;
	virtual void Render(Bullet&) override;

	virtual BulletState* Update(Bullet&) override;
	virtual BulletState* HandleInput(Bullet&, Input&) override;

};


class BulletMState : public BulletState
{

public:

	BulletMState();
	virtual ~BulletMState();

	virtual void Exit(Bullet&) override;
	virtual void Enter(Bullet&) override;
	virtual void Render(Bullet&) override;

	virtual BulletState* Update(Bullet&) override;
	virtual BulletState* HandleInput(Bullet&, Input&) override;

};


class BulletSState : public BulletState
{

public:

	BulletSState();
	virtual ~BulletSState();

	virtual void Exit(Bullet&) override;
	virtual void Enter(Bullet&) override;
	virtual void Render(Bullet&) override;

	static const std::vector<FLOAT> spreadDegrees;
	virtual BulletState* Update(Bullet&) override;
	virtual BulletState* HandleInput(Bullet&, Input&) override;

};


class BulletLState : public BulletState
{

public:

	BulletLState();
	virtual ~BulletLState();

	virtual void Exit(Bullet&) override;
	virtual void Enter(Bullet&) override;
	virtual void Render(Bullet&) override;

	virtual BulletState* Update(Bullet&) override;
	virtual BulletState* HandleInput(Bullet&, Input&) override;

};


class BulletFState : public BulletState
{

public:

	BulletFState
	(
		FLOAT = 0.0f,
		FLOAT = 0.0f,
		FLOAT = 0.0f
	);
	virtual ~BulletFState();

	virtual void Exit(Bullet&) override;
	virtual void Enter(Bullet&) override;
	virtual void Render(Bullet&) override;

	virtual BulletState* Update(Bullet&) override;
	virtual BulletState* HandleInput(Bullet&, Input&) override;

protected:

	FLOAT r;
	FLOAT ω;
	FLOAT dω;
	FLOAT xO;
	FLOAT yO;

};


class BulletEnemyState : public BulletState
{

public:

	BulletEnemyState();
	virtual ~BulletEnemyState();

	virtual void Exit(Bullet&) override;
	virtual void Enter(Bullet&) override;
	virtual void Render(Bullet&) override;

	virtual BulletState* Update(Bullet&) override;
	virtual BulletState* HandleInput(Bullet&, Input&) override;

};


class BulletBossStage1State : public BulletState
{

public:

	BulletBossStage1State();
	virtual ~BulletBossStage1State();

	virtual void Exit(Bullet&) override;
	virtual void Enter(Bullet&) override;
	virtual void Render(Bullet&) override;

	virtual BulletState* Update(Bullet&) override;
	virtual BulletState* HandleInput(Bullet&, Input&) override;

};


class BulletBossStage2StateHand : public BulletState
{

public:

	BulletBossStage2StateHand();
	virtual ~BulletBossStage2StateHand();

	virtual void Exit(Bullet&) override;
	virtual void Enter(Bullet&) override;
	virtual void Render(Bullet&) override;

	virtual BulletState* Update(Bullet&) override;
	virtual BulletState* HandleInput(Bullet&, Input&) override;

};


class BulletBossStage2StateHead : public BulletState
{

public:

	BulletBossStage2StateHead();
	virtual ~BulletBossStage2StateHead();

	virtual void Exit(Bullet&) override;
	virtual void Enter(Bullet&) override;
	virtual void Render(Bullet&) override;

	virtual BulletState* Update(Bullet&) override;
	virtual BulletState* HandleInput(Bullet&, Input&) override;

};


class BulletExplodeState : public BulletState
{

public:

	BulletExplodeState();
	virtual ~BulletExplodeState();

	virtual void Exit(Bullet&) override;
	virtual void Enter(Bullet&) override;
	virtual void Render(Bullet&) override;

	virtual BulletState* Update(Bullet&) override;
	virtual BulletState* HandleInput(Bullet&, Input&) override;

};


class BulletScubaSoldierState : public BulletState
{

public:

	FLOAT θ; FLOAT v0;
	virtual ~BulletScubaSoldierState();
	BulletScubaSoldierState(FLOAT, FLOAT);

	virtual void Exit(Bullet&) override;
	virtual void Enter(Bullet&) override;
	virtual void Render(Bullet&) override;

	virtual BulletState* Update(Bullet&) override;
	virtual BulletState* HandleInput(Bullet&, Input&) override;

};

