#include "Bill.h"

BillStraightUpState::BillStraightUpState() : BillState()
{
}

BillStraightUpState::~BillStraightUpState()
{
}

void BillStraightUpState::Exit(Bill& bill)
{
}

void BillStraightUpState::Enter(Bill& bill)
{
}

void BillStraightUpState::Render(Bill& bill)
{
	bill.SetAnimation(BILL_ANIMATION_ID::STRAIGHT_UP, bill.GetPosition(), bill.GetMovingDirection(), bill.GetAngle());
}

BillState* BillStraightUpState::Update(Bill& bill)
{
	return nullptr;
}

BillState* BillStraightUpState::HandleInput(Bill& bill, Input& input)
{
	if (input.IsKey(DIK_UP))
	{
		if (input.IsKey(DIK_X))
		{
			return new BillShotStraightUpState();
		}
		return nullptr;
	}
	return new BillNormalState();
}
