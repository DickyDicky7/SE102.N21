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
	return NULL;
}

BillState* BillStraightUpState::HandleInput(Bill& bill, Input& input)
{
	if (input.Is(DIK_UP))
	{
		if (input.Is(DIK_X))
		{
			return new BillShotStraightUpState();
		}
		return NULL;
	}
	return new BillNormalState();
}