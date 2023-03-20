#include "Bill.h"


#pragma region
Bill::Bill() : Entity()
{
	self = this;
	this->vx = 2;
	this->vy = 3;
	this->position.x = 100;
	this->position.y = 300;
	this->state = new BillRunState();
	OutputDebugString(L"\n\Bill's constructor called\n\n");
}

Bill::~Bill()
{
	OutputDebugString(L"\n\Bill's destructor called\n\n");
}

void Bill::Update()
{
	BillState* newState = state->Update(*this);
	if (newState)
	{
		state->Exit(*this);
		delete state;
		state = newState;
		state->Enter(*this);
	}
}

void Bill::Render()
{
	state->Render(*this);
}

void Bill::HandleInput()
{
	BillState* newState = state->HandleInput(*this);
	if (newState)
	{
		state->Exit(*this);
		delete state;
		state = newState;
		state->Enter(*this);
	}
}
#pragma endregion Bill


#pragma region
BillState::BillState()
{
}

BillState::~BillState()
{
}
#pragma endregion Bill State


#pragma region
BillRunState::BillRunState()
{
}

BillRunState::~BillRunState()
{
}

void BillRunState::Exit(Bill& bill)
{

}

void BillRunState::Enter(Bill& bill)
{
	bill.SetAnimation(BILL_RUN);
}

void BillRunState::Render(Bill& bill)
{
	bill.SetAnimation(BILL_RUN);
}

BillState* BillRunState::Update(Bill& bill)
{
	bill.SetX(bill.GetX() + bill.GetVX());
	if (bill.GetX() > 300 || bill.GetX() < 10) bill.SetVX(-bill.GetVX());

	return nullptr;
}

BillState* BillRunState::HandleInput(Bill& bill)
{
	return nullptr;
}
#pragma endregion Bill Run State


#pragma region
BillJumpState::BillJumpState()
{
}

BillJumpState::~BillJumpState()
{
}

void BillJumpState::Exit(Bill& bill)
{
	bill.SetAnimation(BILL_NORMAL);
}

void BillJumpState::Enter(Bill& bill)
{

}

void BillJumpState::Render(Bill& bill)
{
	bill.SetAnimation(BILL_JUMP);
}

BillState* BillJumpState::Update(Bill& bill)
{
	return nullptr;
}

BillState* BillJumpState::HandleInput(Bill& bill)
{
	return nullptr;
}
#pragma endregion Bill Jump State






