#include "Bill.h"

BillDiveState::BillDiveState() : BillState()
{
}

BillDiveState::~BillDiveState()
{
}

void BillDiveState::Exit(Bill& bill)
{
}

void BillDiveState::Enter(Bill& bill)
{
}

void BillDiveState::Render(Bill& bill)
{
	bill.SetAnimation(BILL_ANIMATION_ID::DIVE, bill.GetPosition(), bill.GetDirection());
}

BillState* BillDiveState::Update(Bill& bill)
{
	return NULL;
}

BillState* BillDiveState::HandleInput(Bill& bill, Input& input)
{
	if (input.Is(DIK_DOWN))
	{
		if (input.Is(DIK_LEFT))
		{
			bill.SetDirection(DIRECTION::LEFT);
		}
		if (input.Is(DIK_RIGHT))
		{
			bill.SetDirection(DIRECTION::RIGHT);
		}

		return NULL;
	}

	return new BillSwimState();
}