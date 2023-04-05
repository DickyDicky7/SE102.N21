#include "Bill.h"

BillSwimShotStraightUpState::BillSwimShotStraightUpState() : BillState()
{
}

BillSwimShotStraightUpState::~BillSwimShotStraightUpState()
{
}

void BillSwimShotStraightUpState::Exit(Bill& bill)
{
}

void BillSwimShotStraightUpState::Enter(Bill& bill)
{
}

void BillSwimShotStraightUpState::Render(Bill& bill)
{
	bill.SetAnimation(BILL_ANIMATION_ID::SWIM_SHOT_STRAIGHT_UP, bill.GetPosition(), bill.GetMovingDirection(), bill.GetAngle());
}

BillState* BillSwimShotStraightUpState::Update(Bill& bill)
{
	return NULL;
}

BillState* BillSwimShotStraightUpState::HandleInput(Bill& bill, Input& input)
{
	return NULL;
}