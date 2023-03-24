#include "Bill.h"

BillSwimState::BillSwimState(DIRECTION direction) : BillState(direction)
{
}

BillSwimState::~BillSwimState()
{
}

void BillSwimState::Exit(Bill& bill)
{
}

void BillSwimState::Enter(Bill& bill)
{
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
		//direction = LEFT;
		//bill.SetX(bill.GetX() - bill.GetVX());
	}
	if (input.Is(DIK_RIGHT))
	{
		//direction = RIGHT;
		//bill.SetX(bill.GetX() + bill.GetVX());
	}

	if (input.Is(DIK_N))
	{
		return new BillNormalState(direction);
	}

	return NULL;
}