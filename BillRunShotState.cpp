#include "Bill.h"

BillRunShotState::BillRunShotState() : BillState()
{
}

BillRunShotState::~BillRunShotState()
{
}

void BillRunShotState::Exit(Bill& bill)
{
}

void BillRunShotState::Enter(Bill& bill)
{
}

void BillRunShotState::Render(Bill& bill)
{
	bill.SetAnimation(BILL_ANIMATION_ID::RUN_SHOT, bill.GetPosition(), bill.GetMovingDirection());
}

BillState* BillRunShotState::Update(Bill& bill)
{
	return NULL;
}

BillState* BillRunShotState::HandleInput(Bill& bill, Input& input)
{
	return NULL;
}