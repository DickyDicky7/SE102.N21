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
	bill.SetAnimation(BILL_ANIMATION_ID::LAYDOWN, bill.GetPosition(), bill.GetMovingDirection(), bill.GetAngle());
}

BillState* BillLayDownState::Update(Bill& bill)
{
	return NULL;
}

BillState* BillLayDownState::HandleInput(Bill& bill, Input& input)
{
	if (input.IsKey(DIK_DOWN))
	{
		if (input.IsKey(DIK_X)) bill.Fire(); return NULL;
	}
	return new BillNormalState();
}