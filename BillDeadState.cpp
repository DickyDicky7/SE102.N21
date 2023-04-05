#include "Bill.h"

BillDeadState::BillDeadState() : BillState()
{
}

BillDeadState::~BillDeadState()
{
}

void BillDeadState::Exit(Bill& bill)
{
}

void BillDeadState::Enter(Bill& bill)
{
}

void BillDeadState::Render(Bill& bill)
{
	bill.SetAnimation(BILL_ANIMATION_ID::DEAD, bill.GetPosition(), bill.GetMovingDirection(), bill.GetAngle());
}

BillState* BillDeadState::Update(Bill& bill)
{
	return NULL;
}

BillState* BillDeadState::HandleInput(Bill& bill, Input& input)
{
	return NULL;
}