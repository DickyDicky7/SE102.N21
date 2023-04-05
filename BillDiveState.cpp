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
	bill.SetAnimation(BILL_ANIMATION_ID::DIVE, bill.GetPosition(), bill.GetMovingDirection(), bill.GetAngle());
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
			bill.SetMovingDirection(DIRECTION::LEFT);
		}
		if (input.Is(DIK_RIGHT))
		{
			bill.SetMovingDirection(DIRECTION::RIGHT);
		}

		return NULL;
	}

	return new BillSwimNormalState();
}