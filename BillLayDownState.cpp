#include "Bill.h"

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