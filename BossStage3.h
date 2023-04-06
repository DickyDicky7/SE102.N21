#pragma once
#include "State.h"
#include "Common.h"
#include "Entity.h"
#include "HasAnimations.h"
#include "HasSprites.h"
#include "HasTextures.h"
#include "Bill.h"

class BossStage3;
class BossStage3State;
class BossStage3OpenState;
class BossStage3CloseState;


class BossStage3 : public Entity<BossStage3>, public HasTextures<BossStage3>, public HasSprites<BossStage3>, public HasAnimations<BossStage3>
{
public:
	BossStage3();
	virtual ~BossStage3();
	void Update() override;
	void Render() override;
	void HandleInput(Input&) override;

	void LoadSprites() override;
	void LoadTextures() override;
	void LoadAnimations() override;

protected:
	BossStage3State* state;
	BossStage3State* updateState;
	BossStage3State* handleInputState;
};



class BossStage3State : public State<BossStage3State, BossStage3>
{

public:

	BossStage3State();
	virtual ~BossStage3State();

	virtual void Exit(BossStage3&) override = 0;
	virtual void Enter(BossStage3&) override = 0;
	virtual void Render(BossStage3&) override = 0;

	virtual BossStage3State* Update(BossStage3&) override = 0;
	virtual BossStage3State* HandleInput(BossStage3&, Input&) override = 0;
};


class BossStage3OpenState : public BossStage3State
{

public:

	BossStage3OpenState();
	virtual ~BossStage3OpenState();

	virtual void Exit(BossStage3&) override;
	virtual void Enter(BossStage3&) override;
	virtual void Render(BossStage3&) override;

	virtual BossStage3State* Update(BossStage3&) override;
	virtual BossStage3State* HandleInput(BossStage3&, Input&) override;
};


class BossStage3CloseState : public BossStage3State
{

public:

	BossStage3CloseState();
	virtual ~BossStage3CloseState();

	virtual void Exit(BossStage3&) override;
	virtual void Enter(BossStage3&) override;
	virtual void Render(BossStage3&) override;

	virtual BossStage3State* Update(BossStage3&) override;
	virtual BossStage3State* HandleInput(BossStage3&, Input&) override;
};