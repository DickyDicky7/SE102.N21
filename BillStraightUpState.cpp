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
	bill.SetAnimation(BILL_ANIMATION_ID::STRAIGHT_UP, bill.GetPosition(), bill.GetMovingDirection());
}

BillState* BillStraightUpState::Update(Bill& bill)
{
	return NULL;
}

BillState* BillStraightUpState::HandleInput(Bill& bill, Input& input)
{
	return NULL;
}