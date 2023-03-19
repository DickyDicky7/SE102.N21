#pragma once


#include "State.h"
#include "Common.h"
#include "Entity.h"


class BillState;
class BillRunState;
class BillJumpState;


class Bill : public Entity<Bill>
{

public:

	Bill();
	virtual ~Bill();
	void Update() override;
	void Render() override;
	void HandleInput() override;

protected:

	BillState* state;

};


class BillState : public State<BillState, Bill>
{

public:

	BillState();
	virtual ~BillState();

	virtual void Exit(Bill&) override = 0;
	virtual void Enter(Bill&) override = 0;
	virtual void Render(Bill&) override = 0;

	virtual BillState* Update(Bill&) override = 0;
	virtual BillState* HandleInput(Bill&) override = 0;

};


class BillRunState : public BillState
{

public:

	BillRunState();
	virtual ~BillRunState();

	virtual void Exit(Bill&) override;
	virtual void Enter(Bill&) override;
	virtual void Render(Bill&) override;

	virtual BillState* Update(Bill&) override;
	virtual BillState* HandleInput(Bill&) override;

};


class BillJumpState : public BillState
{

public:

	BillJumpState();
	virtual ~BillJumpState();

	virtual void Exit(Bill&) override;
	virtual void Enter(Bill&) override;
	virtual void Render(Bill&) override;

	virtual BillState* Update(Bill&) override;
	virtual BillState* HandleInput(Bill&) override;

};








