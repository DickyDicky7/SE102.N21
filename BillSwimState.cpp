#include "Bill.h"

BillSwimState::BillSwimState() : BillState()
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
	bill.SetAnimation(BILL_ANIMATION_ID::SWIM, bill.GetPosition(), bill.GetMovingDirection());
}

BillState* BillSwimState::Update(Bill& bill)
{
	return NULL;
}

BillState* BillSwimState::HandleInput(Bill& bill, Input& input)
{
	if (input.Is(DIK_LEFT))
	{
		bill.SetMovingDirection(DIRECTION::LEFT);
	}
	if (input.Is(DIK_RIGHT))
	{
		bill.SetMovingDirection(DIRECTION::RIGHT);
	}
	if (input.Is(DIK_DOWN))
	{
		return new BillDiveState();
	}

	if (input.Is(DIK_N))
	{
		return new BillNormalState();
	}

	return NULL;
}