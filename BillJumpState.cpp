#include "Bill.h"

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