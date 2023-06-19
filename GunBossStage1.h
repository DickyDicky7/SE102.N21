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

class GunBossStage1;
class GunBossStage1State;
class GunBossStage1NormalState;
class GunBossStage1PrepareShootState;
class GunBossStage1DestroyState;

class GunBossStage1 : public Entity, public Enemy<Bill>
	, public HasTextures<GunBossStage1>, public HasSprites<GunBossStage1>, public HasAnimations<GunBossStage1>, public BossStage1
{
public:
	GunBossStage1();
	GunBossStage1(int);
	~GunBossStage1();

	virtual void Update() override;
	virtual void Render() override;
	virtual void HandleInput(Input&) override;

	void LoadTextures() override;
	void LoadSprites() override;
	void LoadAnimations() override;
	int GetType();

protected:
	GunBossStage1State* state;
	GunBossStage1State* updateState;
	int type;
};

class GunBossStage1State : public State<GunBossStage1State, GunBossStage1>
{
public:
	GunBossStage1State();
	~GunBossStage1State();

	virtual void Exit(GunBossStage1&) = 0;
	virtual void Enter(GunBossStage1&) = 0;
	virtual void Render(GunBossStage1&) = 0;

	virtual GunBossStage1State* Update(GunBossStage1&) = 0;
	virtual GunBossStage1State* HandleInput(GunBossStage1&, Input&) override;

protected:
	FLOAT time;
};

class GunBossStage1NormalState : public GunBossStage1State
{
public:
	GunBossStage1NormalState();
	GunBossStage1NormalState(FLOAT);
	~GunBossStage1NormalState();

	virtual void Exit(GunBossStage1&);
	virtual void Enter(GunBossStage1&);
	virtual void Render(GunBossStage1&);

	virtual GunBossStage1State* Update(GunBossStage1&);

};

class GunBossStage1PrepareShootState : public GunBossStage1State
{
public:
	GunBossStage1PrepareShootState();
	GunBossStage1PrepareShootState(FLOAT);
	~GunBossStage1PrepareShootState();

	virtual void Exit(GunBossStage1&);
	virtual void Enter(GunBossStage1&);
	virtual void Render(GunBossStage1&);

	virtual GunBossStage1State* Update(GunBossStage1&);
};


class GunBossStage1DestroyState : public GunBossStage1State
{
public:
	GunBossStage1DestroyState();
	~GunBossStage1DestroyState();

	virtual void Exit(GunBossStage1&);
	virtual void Enter(GunBossStage1&);
	virtual void Render(GunBossStage1&);

	virtual GunBossStage1State* Update(GunBossStage1&);

protected:
	boolean isDestroy;
	boolean isInDestroyPos;
};

