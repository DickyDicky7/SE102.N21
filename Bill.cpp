#include "Bill.h"


#pragma region
Bill::Bill() : Entity(), HasAnimations()
{
	self = this;
	this->vx = 3;
	this->vy = 7;
	this->position.x = 50;
	this->position.y = 150;
	this->state = new BillNormalState(RIGHT);
	this->updateState = NULL;
	this->handleInputState = NULL;
	OutputDebugString(L"\n\nBill's constructor called\n\n");
}

Bill::~Bill()
{
	OutputDebugString(L"\n\nBill's destructor called\n\n");
}

void Bill::Update()
{
	updateState = state->Update(*this);
}

void Bill::Render()
{
	state->Render(*this);
	if (updateState)
	{
		state->Exit(*this);
		delete state;
		state = updateState;
		state->Enter(*this);
		updateState = NULL;
	}
	if (handleInputState)
	{
		state->Exit(*this);
		delete state;
		state = handleInputState;
		state->Enter(*this);
		handleInputState = NULL;
	}
}

void Bill::HandleInput(Input& input)
{
	handleInputState = state->HandleInput(*this, input);
}
#pragma endregion Bill


#pragma region
BillState::BillState(DIRECTION direction)
{
	this->direction = direction;
}

BillState::~BillState()
{
}
#pragma endregion Bill State


#pragma region
BillRunState::BillRunState(DIRECTION direction) : BillState(direction)
{
}

BillRunState::~BillRunState()
{
}

void BillRunState::Exit(Bill& bill)
{
	bill.SetAnimation(BILL_RUN, bill.GetPosition(), direction);
}

void BillRunState::Enter(Bill& bill)
{
	bill.SetAnimation(BILL_RUN, bill.GetPosition(), direction);
}

void BillRunState::Render(Bill& bill)
{
	bill.SetAnimation(BILL_RUN, bill.GetPosition(), direction);
}

BillState* BillRunState::Update(Bill& bill)
{
	if (direction == LEFT)
		bill.SetX(bill.GetX() - bill.GetVX());
	if (direction == RIGHT)
		bill.SetX(bill.GetX() + bill.GetVX());
	return NULL;
}

BillState* BillRunState::HandleInput(Bill& bill, Input& input)
{
	if (input.Is(DIK_A) || input.Is(DIK_D))
	{
		return NULL;
	}
	return new BillNormalState(direction);
}
#pragma endregion Bill Run State


#pragma region
BillJumpState::BillJumpState(DIRECTION direction) : BillState(direction)
{
}

BillJumpState::~BillJumpState()
{
}

void BillJumpState::Exit(Bill& bill)
{
	bill.SetAnimation(BILL_JUMP, bill.GetPosition(), direction);
}

void BillJumpState::Enter(Bill& bill)
{
	bill.SetAnimation(BILL_JUMP, bill.GetPosition(), direction);
}

void BillJumpState::Render(Bill& bill)
{
	bill.SetAnimation(BILL_JUMP, bill.GetPosition(), direction);
}

BillState* BillJumpState::Update(Bill& bill)
{
	if (time == 20 || time == 0)
		bill.SetY(bill.GetY() - bill.GetVY());
	if (bill.GetY() <= 100)
	{
		if (time != 0)
			time--;
		if (time == 0)
			bill.SetVY(-bill.GetVY());
	}
	if (bill.GetY() >= 150)
	{
		bill.SetY(150);
		bill.SetVY(-bill.GetVY());
		return new BillNormalState(direction);
	}
	return NULL;
}

BillState* BillJumpState::HandleInput(Bill& bill, Input& input)
{
	if (input.Is(DIK_A))
	{
		bill.SetX(bill.GetX() - bill.GetVX());
	}
	if (input.Is(DIK_D))
	{
		bill.SetX(bill.GetX() + bill.GetVX());
	}
	return NULL;
}
#pragma endregion Bill Jump State


#pragma region
BillNormalState::BillNormalState(DIRECTION direction) : BillState(direction)
{
}

BillNormalState::~BillNormalState()
{
}

void BillNormalState::Exit(Bill& bill)
{
	bill.SetAnimation(BILL_NORMAL, bill.GetPosition(), direction);
}

void BillNormalState::Enter(Bill& bill)
{
	bill.SetAnimation(BILL_NORMAL, bill.GetPosition(), direction);
}

void BillNormalState::Render(Bill& bill)
{
	bill.SetAnimation(BILL_NORMAL, bill.GetPosition(), direction);
}

BillState* BillNormalState::Update(Bill& bill)
{
	return NULL;
}

BillState* BillNormalState::HandleInput(Bill& bill, Input& input)
{
	if (input.Is(DIK_A))
	{
		OutputDebugString(L"A entered\n");
		return new BillRunState(LEFT);
	}
	if (input.Is(DIK_D))
	{
		OutputDebugString(L"D entered\n");
		return new BillRunState(RIGHT);
	}
	if (input.Is(DIK_W))
	{
		OutputDebugString(L"W entered\n");
		return new BillJumpState(direction);
	}
	if (input.Is(DIK_S))
	{
		OutputDebugString(L"S entered\n");
		return new BillLayDownState(direction);
	}
	return NULL;
}
#pragma endregion Bill Normal State


#pragma region
BillLayDownState::BillLayDownState(DIRECTION direction) : BillState(direction)
{
}

BillLayDownState::~BillLayDownState()
{
}

void BillLayDownState::Exit(Bill& bill)
{
	bill.SetAnimation(BILL_LAYDOWN, bill.GetPosition(), direction);
}

void BillLayDownState::Enter(Bill& bill)
{
	bill.SetAnimation(BILL_LAYDOWN, bill.GetPosition(), direction);
}

void BillLayDownState::Render(Bill& bill)
{
	bill.SetAnimation(BILL_LAYDOWN, bill.GetPosition(), direction);
}

BillState* BillLayDownState::Update(Bill& bill)
{
	return NULL;
}

BillState* BillLayDownState::HandleInput(Bill& bill, Input& input)
{
	if (input.Is(DIK_S))
	{
		return NULL;
	}
	return new BillNormalState(direction);
}
#pragma endregion Bill LayDown State










