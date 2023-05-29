#pragma once
#include "State.h"
#include "Common.h"
#include "Entity.h"
#include "HasAnimations.h"
#include "HasSprites.h"
#include "HasTextures.h"
#include "Bill.h"
#include "Enemy.h"

class BossStage3Hand;
class BossStage3HandState;
class BossStage3HandLeftState;
class BossStage3HandRightState;


class BossStage3Hand : public Entity, public Enemy<Bill>
	, public HasTextures<BossStage3Hand>, public HasSprites<BossStage3Hand>, public HasAnimations<BossStage3Hand>
{
public:
	BossStage3Hand();
	virtual ~BossStage3Hand();
	void Update() override;
	void Render() override;
	void HandleInput(Input&) override;

	void LoadSprites() override;
	void LoadTextures() override;
	void LoadAnimations() override;

protected:
	BossStage3HandState* state;
	BossStage3HandState* updateState;
	BossStage3HandState* handleInputState;
};



class BossStage3HandState : public State<BossStage3HandState, BossStage3Hand>
{

public:

	BossStage3HandState();
	virtual ~BossStage3HandState();

	virtual void Exit(BossStage3Hand&) override = 0;
	virtual void Enter(BossStage3Hand&) override = 0;
	virtual void Render(BossStage3Hand&) override = 0;

	virtual BossStage3HandState* Update(BossStage3Hand&) override = 0;
	virtual BossStage3HandState* HandleInput(BossStage3Hand&, Input&) override = 0;
};


class BossStage3HandOpenState : public BossStage3HandState
{

public:

	BossStage3HandOpenState();
	virtual ~BossStage3HandOpenState();

	virtual void Exit(BossStage3Hand&) override;
	virtual void Enter(BossStage3Hand&) override;
	virtual void Render(BossStage3Hand&) override;

	virtual BossStage3HandState* Update(BossStage3Hand&) override;
	virtual BossStage3HandState* HandleInput(BossStage3Hand&, Input&) override;
};


class BossStage3HandCloseState : public BossStage3HandState
{

public:

	BossStage3HandCloseState();
	virtual ~BossStage3HandCloseState();

	virtual void Exit(BossStage3Hand&) override;
	virtual void Enter(BossStage3Hand&) override;
	virtual void Render(BossStage3Hand&) override;

	virtual BossStage3HandState* Update(BossStage3Hand&) override;
	virtual BossStage3HandState* HandleInput(BossStage3Hand&, Input&) override;
};

class BossStage3HandMiddleState : public BossStage3HandState
{

public:

	BossStage3HandMiddleState(BOSS_STAGE_3_ANIMATION_ID nextState);
	virtual ~BossStage3HandMiddleState();

	virtual void Exit(BossStage3Hand&) override;
	virtual void Enter(BossStage3Hand&) override;
	virtual void Render(BossStage3Hand&) override;

	virtual BossStage3HandState* Update(BossStage3Hand&) override;
	virtual BossStage3HandState* HandleInput(BossStage3Hand&, Input&) override;
protected:
	TIME delayTime;
	BOSS_STAGE_3_ANIMATION_ID nextState;
};