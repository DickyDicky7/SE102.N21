#pragma once
#include "State.h"
#include "Common.h"
#include "Entity.h"
#include "HasAnimations.h"
#include "HasSprites.h"
#include "HasTextures.h"
#include "Bill.h"
#include "Enemy.h"
#include "BossStage3.h"

class BossStage3Gate;
class BossStage3GateState;
class BossStage3GateOpenState;
class BossStage3GateCloseState;


class BossStage3Gate : public Entity
	, public HasTextures<BossStage3Gate>, public HasSprites<BossStage3Gate>, public HasAnimations<BossStage3Gate>
{
public:
	BossStage3Gate();
	virtual ~BossStage3Gate();
	void Update() override;
	void Render() override;
	void HandleInput(Input&) override;

	void LoadSprites() override;
	void LoadTextures() override;
	void LoadAnimations() override;

	BossStage3* bossStage3Head;

	void SetHead(BossStage3* head) { bossStage3Head = head; }
	BOOL IsDead();

protected:
	BossStage3GateState* state;
	BossStage3GateState* updateState;
	BossStage3GateState* handleInputState;
};



class BossStage3GateState : public State<BossStage3GateState, BossStage3Gate>
{

public:

	BossStage3GateState();
	virtual ~BossStage3GateState();

	virtual void Exit(BossStage3Gate&) override = 0;
	virtual void Enter(BossStage3Gate&) override = 0;
	virtual void Render(BossStage3Gate&) override = 0;

	virtual BossStage3GateState* Update(BossStage3Gate&) override = 0;
	virtual BossStage3GateState* HandleInput(BossStage3Gate&, Input&) override = 0;
};


class BossStage3GateOpenState : public BossStage3GateState
{

public:

	BossStage3GateOpenState();
	virtual ~BossStage3GateOpenState();

	virtual void Exit(BossStage3Gate&) override;
	virtual void Enter(BossStage3Gate&) override;
	virtual void Render(BossStage3Gate&) override;

	virtual BossStage3GateState* Update(BossStage3Gate&) override;
	virtual BossStage3GateState* HandleInput(BossStage3Gate&, Input&) override;
};


class BossStage3GateCloseState : public BossStage3GateState
{

public:

	BossStage3GateCloseState();
	virtual ~BossStage3GateCloseState();

	virtual void Exit(BossStage3Gate&) override;
	virtual void Enter(BossStage3Gate&) override;
	virtual void Render(BossStage3Gate&) override;

	virtual BossStage3GateState* Update(BossStage3Gate&) override;
	virtual BossStage3GateState* HandleInput(BossStage3Gate&, Input&) override;
};