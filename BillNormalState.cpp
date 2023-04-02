#include "Bill.h"

BillNormalState::BillNormalState() : BillState()
{
}

BillNormalState::~BillNormalState()
{
}

void BillNormalState::Exit(Bill& bill)
{
}

void BillNormalState::Enter(Bill& bill)
{
}

void BillNormalState::Render(Bill& bill)
{
	bill.SetAnimation(BILL_ANIMATION_ID::NORMAL, bill.GetPosition(), bill.GetDirection());
}

BillState* BillNormalState::Update(Bill& bill)
{
	return NULL;
}

BillState* BillNormalState::HandleInput(Bill& bill, Input& input)
{
	if (input.Is(DIK_LEFT))
	{
		bill.SetDirection(DIRECTION::LEFT);
		return new BillRunState();
	}
	if (input.Is(DIK_RIGHT))
	{
		bill.SetDirection(DIRECTION::RIGHT);
		return new BillRunState();
	}
	if (input.Is(DIK_Z))
	{
		return new BillJumpState();
	}
	if (input.Is(DIK_DOWN))
	{
		return new BillLayDownState();
	}

	if (input.Is(DIK_B))
	{
		return new BillSwimState();
	}

	return NULL;
}