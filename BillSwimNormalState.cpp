#include "Bill.h"

BillSwimNormalState::BillSwimNormalState() : BillState()
{
}

BillSwimNormalState::~BillSwimNormalState()
{
}

void BillSwimNormalState::Exit(Bill& bill)
{
}

void BillSwimNormalState::Enter(Bill& bill)
{
}

void BillSwimNormalState::Render(Bill& bill)
{
	bill.SetAnimation(BILL_ANIMATION_ID::SWIM_NORMAL, bill.GetPosition(), bill.GetMovingDirection());
}

BillState* BillSwimNormalState::Update(Bill& bill)
{
	return NULL;
}

BillState* BillSwimNormalState::HandleInput(Bill& bill, Input& input)
{
	if (input.Is(DIK_LEFT))
	{
		bill.SetMovingDirection(DIRECTION::LEFT);
		return new BillSwimRunState();
	}
	if (input.Is(DIK_RIGHT))
	{
		bill.SetMovingDirection(DIRECTION::RIGHT);
		return new BillSwimRunState();
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