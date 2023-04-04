#include "Bill.h"

BillSwimShotState::BillSwimShotState() : BillState()
{
}

BillSwimShotState::~BillSwimShotState()
{
}

void BillSwimShotState::Exit(Bill& bill)
{
}

void BillSwimShotState::Enter(Bill& bill)
{
}

void BillSwimShotState::Render(Bill& bill)
{
	bill.SetAnimation(BILL_ANIMATION_ID::SWIM_SHOT, bill.GetPosition(), bill.GetMovingDirection());
}

BillState* BillSwimShotState::Update(Bill& bill)
{
	return NULL;
}

BillState* BillSwimShotState::HandleInput(Bill& bill, Input& input)
{
	return NULL;
}