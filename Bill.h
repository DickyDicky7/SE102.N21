#pragma once


#include "State.h"
#include "Common.h"
#include "Entity.h"
#include "HasAnimations.h"


class BillState;
class BillRunState;
class BillJumpState;
class BillSwimState;
class BillNormalState;
class BillLayDownState;
class BillRunShotAngleUpState;
class BillRunShotAngleDownState;


class Bill : public Entity<Bill>, public HasAnimations
{

public:

	Bill();
	virtual ~Bill();
	void Update() override;
	void Render() override;
	void HandleInput(Input&) override;

protected:

	BillState* state;
	BillState* updateState;
	BillState* handleInputState;

};


class BillState : public State<BillState, Bill>
{

public:

	BillState(DIRECTION);
	virtual ~BillState();

	virtual void Exit(Bill&) override = 0;
	virtual void Enter(Bill&) override = 0;
	virtual void Render(Bill&) override = 0;

	virtual BillState* Update(Bill&) override = 0;
	virtual BillState* HandleInput(Bill&, Input&) override = 0;

protected:

	FLOAT time;
	DIRECTION direction;

};


class BillRunState : public BillState
{

public:

	BillRunState(DIRECTION);
	virtual ~BillRunState();

	virtual void Exit(Bill&) override;
	virtual void Enter(Bill&) override;
	virtual void Render(Bill&) override;

	virtual BillState* Update(Bill&) override;
	virtual BillState* HandleInput(Bill&, Input&) override;

};


class BillJumpState : public BillState
{

public:

	BillJumpState(DIRECTION);
	virtual ~BillJumpState();

	virtual void Exit(Bill&) override;
	virtual void Enter(Bill&) override;
	virtual void Render(Bill&) override;

	virtual BillState* Update(Bill&) override;
	virtual BillState* HandleInput(Bill&, Input&) override;

protected:

	BOOL hasMovedLeft;
	BOOL hasMovedRight;

};


class BillSwimState : public BillState
{

public:

	BillSwimState(DIRECTION);
	virtual ~BillSwimState();

	virtual void Exit(Bill&) override;
	virtual void Enter(Bill&) override;
	virtual void Render(Bill&) override;

	virtual BillState* Update(Bill&) override;
	virtual BillState* HandleInput(Bill&, Input&) override;

};


class BillNormalState : public BillState
{

public:

	BillNormalState(DIRECTION);
	virtual ~BillNormalState();

	virtual void Exit(Bill&) override;
	virtual void Enter(Bill&) override;
	virtual void Render(Bill&) override;

	virtual BillState* Update(Bill&) override;
	virtual BillState* HandleInput(Bill&, Input&) override;

};


class BillLayDownState : public BillState
{

public:

	BillLayDownState(DIRECTION);
	virtual ~BillLayDownState();

	virtual void Exit(Bill&) override;
	virtual void Enter(Bill&) override;
	virtual void Render(Bill&) override;

	virtual BillState* Update(Bill&) override;
	virtual BillState* HandleInput(Bill&, Input&) override;

};


class BillRunShotAngleUpState : public BillState
{

public:

	BillRunShotAngleUpState(DIRECTION);
	virtual ~BillRunShotAngleUpState();

	virtual void Exit(Bill&) override;
	virtual void Enter(Bill&) override;
	virtual void Render(Bill&) override;

	virtual BillState* Update(Bill&) override;
	virtual BillState* HandleInput(Bill&, Input&) override;

};


class BillRunShotAngleDownState : public BillState
{

public:

	BillRunShotAngleDownState(DIRECTION);
	virtual ~BillRunShotAngleDownState();

	virtual void Exit(Bill&) override;
	virtual void Enter(Bill&) override;
	virtual void Render(Bill&) override;

	virtual BillState* Update(Bill&) override;
	virtual BillState* HandleInput(Bill&, Input&) override;

};



















