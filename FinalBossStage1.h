#pragma once
#include "State.h"
#include "Common.h"
#include "Entity.h"
#include "HasAnimations.h"
#include "HasSprites.h"
#include "HasTextures.h"
#include "Bill.h"
#include "Enemy.h"
#include "BossStage1.h"
#include "ExplosionCommon.h"
#include "GunBossStage1.h"

class FinalBossStage1;
class FinalBossStage1State;
class FinalBossStage1NormalState;
class FinalBossStage1DestroyState;

class FinalBossStage1 : public Entity, public Enemy<Bill>
	, public HasTextures<FinalBossStage1>, public HasSprites<FinalBossStage1>, public HasAnimations<FinalBossStage1>, public BossStage1
{
public:
	FinalBossStage1();
	~FinalBossStage1();

	virtual void Update() override;
	virtual void Render() override;
	virtual void HandleInput(Input&) override;

	void LoadTextures() override;
	void LoadSprites() override;
	void LoadAnimations() override;

	void SetGun1(GunBossStage1*);
	void SetGun2(GunBossStage1*);

protected:
	FinalBossStage1State* state;
	FinalBossStage1State* updateState;

	GunBossStage1* gun1;
	GunBossStage1* gun2;
};

class FinalBossStage1State : public State<FinalBossStage1State, FinalBossStage1>
{
public:
	FinalBossStage1State();
	~FinalBossStage1State();

	virtual void Exit(FinalBossStage1&) = 0;
	virtual void Enter(FinalBossStage1&) = 0;
	virtual void Render(FinalBossStage1&) = 0;

	virtual FinalBossStage1State* Update(FinalBossStage1&) = 0;
	virtual FinalBossStage1State* HandleInput(FinalBossStage1&, Input&) override;

protected:
	FLOAT time;
};

class FinalBossStage1NormalState : public FinalBossStage1State
{
public:
	FinalBossStage1NormalState();
	~FinalBossStage1NormalState();

	virtual void Exit(FinalBossStage1&);
	virtual void Enter(FinalBossStage1&);
	virtual void Render(FinalBossStage1&);

	virtual FinalBossStage1State* Update(FinalBossStage1&);
};

class FinalBossStage1DestroyState : public FinalBossStage1State
{
public:
	FinalBossStage1DestroyState();
	~FinalBossStage1DestroyState();

	virtual void Exit(FinalBossStage1&);
	virtual void Enter(FinalBossStage1&);
	virtual void Render(FinalBossStage1&);

	virtual FinalBossStage1State* Update(FinalBossStage1&);
protected:
	boolean isDestroy;
};