#include "Bill.h"

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