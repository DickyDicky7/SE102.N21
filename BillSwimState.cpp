#include "Bill.h"


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
	//for (int i = 0; i <= 20; i++)
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