#include "Bill.h"

BillShotStraightUpState::BillShotStraightUpState() : BillState()
{
}

BillShotStraightUpState::~BillShotStraightUpState()
{
}

void BillShotStraightUpState::Exit(Bill& bill)
{
}

void BillShotStraightUpState::Enter(Bill& bill)
{
}

void BillShotStraightUpState::Render(Bill& bill)
{
	bill.SetAnimation(BILL_ANIMATION_ID::SHOT_STRAIGHT_UP, bill.GetPosition(), bill.GetMovingDirection(), bill.GetAngle());
}

BillState* BillShotStraightUpState::Update(Bill& bill)
{
	return NULL;
}

BillState* BillShotStraightUpState::HandleInput(Bill& bill, Input& input)
{
	if (input.IsKey(DIK_X))
	{
		return NULL;
	}
	return new BillStraightUpState();
}