#include "Bill.h"


#pragma region
Bill::Bill() : Entity(), HasAnimations()
{
	self = this;
	this->vx = 2;
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
	if (input.Is(DIK_LEFT) || input.Is(DIK_RIGHT))
	{
		if (input.Is(DIK_Z))
		{
			return new BillJumpState(direction);
		}
		if (input.Is(DIK_UP))
		{
			return new BillRunShotAngleUpState(direction);
		}
		if (input.Is(DIK_DOWN))
		{
			return new BillRunShotAngleDownState(direction);
		}
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
	if (input.Is(DIK_LEFT))
	{
		bill.SetX(bill.GetX() - bill.GetVX());
	}
	if (input.Is(DIK_RIGHT))
	{
		bill.SetX(bill.GetX() + bill.GetVX());
	}
	return NULL;
}
#pragma endregion Bill Jump State


#pragma region
BillSwimState::BillSwimState(DIRECTION direction) : BillState(direction)
{
}

BillSwimState::~BillSwimState()
{
}

void BillSwimState::Exit(Bill& bill)
{
	bill.SetAnimation(BILL_SWIM, bill.GetPosition(), direction);
}

void BillSwimState::Enter(Bill& bill)
{
	for (int i = 0; i <= 20; i++)
		bill.SetAnimation(BILL_BEGIN_SWIM, bill.GetPosition(), direction);
}

void BillSwimState::Render(Bill& bill)
{
	bill.SetAnimation(BILL_SWIM, bill.GetPosition(), direction);
}

BillState* BillSwimState::Update(Bill& bill)
{
	return NULL;
}

BillState* BillSwimState::HandleInput(Bill& bill, Input& input)
{
	if (input.Is(DIK_LEFT))
	{
		direction = LEFT;
		bill.SetX(bill.GetX() - bill.GetVX());
	}
	if (input.Is(DIK_RIGHT))
	{
		direction = RIGHT;
		bill.SetX(bill.GetX() + bill.GetVX());
	}

	if (input.Is(DIK_N))
	{
		return new BillNormalState(direction);
	}

	return NULL;
}
#pragma endregion Bill Swim State


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
	if (input.Is(DIK_LEFT))
	{
		OutputDebugString(L"LEFT entered\n");
		return new BillRunState(LEFT);
	}
	if (input.Is(DIK_RIGHT))
	{
		OutputDebugString(L"RIGHT entered\n");
		return new BillRunState(RIGHT);
	}
	if (input.Is(DIK_Z))
	{
		OutputDebugString(L"Z entered\n");
		return new BillJumpState(direction);
	}
	if (input.Is(DIK_DOWN))
	{
		OutputDebugString(L"DOWN entered\n");
		return new BillLayDownState(direction);
	}

	if (input.Is(DIK_B))
	{
		OutputDebugString(L"B entered\n");
		return new BillSwimState(direction);
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
	if (input.Is(DIK_DOWN))
	{
		return NULL;
	}
	return new BillNormalState(direction);
}
#pragma endregion Bill LayDown State


#pragma region
BillRunShotAngleUpState::BillRunShotAngleUpState(DIRECTION direction) : BillState(direction)
{
}

BillRunShotAngleUpState::~BillRunShotAngleUpState()
{
}

void BillRunShotAngleUpState::Exit(Bill& bill)
{
	bill.SetAnimation(BILL_RUN_SHOT_ANGLE_UP, bill.GetPosition(), direction);
}

void BillRunShotAngleUpState::Enter(Bill& bill)
{
	bill.SetAnimation(BILL_RUN_SHOT_ANGLE_UP, bill.GetPosition(), direction);
}

void BillRunShotAngleUpState::Render(Bill& bill)
{
	bill.SetAnimation(BILL_RUN_SHOT_ANGLE_UP, bill.GetPosition(), direction);
}

BillState* BillRunShotAngleUpState::Update(Bill& bill)
{
	if (direction == LEFT)
		bill.SetX(bill.GetX() - bill.GetVX());
	if (direction == RIGHT)
		bill.SetX(bill.GetX() + bill.GetVX());
	return NULL;
}

BillState* BillRunShotAngleUpState::HandleInput(Bill& bill, Input& input)
{
	if (input.Is(DIK_LEFT) || input.Is(DIK_RIGHT))
	{
		if (input.Is(DIK_UP))
		{
			return NULL;
		}
	}
	return new BillNormalState(direction);
}
#pragma endregion Bill Run Shot Angle Up State


#pragma region
BillRunShotAngleDownState::BillRunShotAngleDownState(DIRECTION direction) : BillState(direction)
{
}

BillRunShotAngleDownState::~BillRunShotAngleDownState()
{
}

void BillRunShotAngleDownState::Exit(Bill& bill)
{
	bill.SetAnimation(BILL_RUN_SHOT_ANGLE_DOWN, bill.GetPosition(), direction);
}

void BillRunShotAngleDownState::Enter(Bill& bill)
{
	bill.SetAnimation(BILL_RUN_SHOT_ANGLE_DOWN, bill.GetPosition(), direction);
}

void BillRunShotAngleDownState::Render(Bill& bill)
{
	bill.SetAnimation(BILL_RUN_SHOT_ANGLE_DOWN, bill.GetPosition(), direction);
}

BillState* BillRunShotAngleDownState::Update(Bill& bill)
{
	if (direction == LEFT)
		bill.SetX(bill.GetX() - bill.GetVX());
	if (direction == RIGHT)
		bill.SetX(bill.GetX() + bill.GetVX());
	return NULL;
}

BillState* BillRunShotAngleDownState::HandleInput(Bill& bill, Input& input)
{

	if (input.Is(DIK_LEFT) || input.Is(DIK_RIGHT))
	{
		if (input.Is(DIK_DOWN))
		{
			return NULL;
		}
	}
	return new BillNormalState(direction);
}
#pragma endregion Bill Run Shot Angle Down State





















