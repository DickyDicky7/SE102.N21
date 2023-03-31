#include "Bill.h"

BillLayDownState::BillLayDownState() : BillState()
{
}

BillLayDownState::~BillLayDownState()
{
}

void BillLayDownState::Exit(Bill& bill)
{
}

void BillLayDownState::Enter(Bill& bill)
{
}

void BillLayDownState::Render(Bill& bill)
{
	bill.SetAnimation(BILL_LAYDOWN, bill.GetPosition(), bill.GetDirection());
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
	return new BillNormalState();
}