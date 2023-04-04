#include "Bill.h"

BillNormalShotState::BillNormalShotState() : BillState()
{
}

BillNormalShotState::~BillNormalShotState()
{
}

void BillNormalShotState::Exit(Bill& bill)
{
}

void BillNormalShotState::Enter(Bill& bill)
{
}

void BillNormalShotState::Render(Bill& bill)
{
	bill.SetAnimation(BILL_ANIMATION_ID::NORMAL_SHOT, bill.GetPosition(), bill.GetMovingDirection());
}

BillState* BillNormalShotState::Update(Bill& bill)
{
	return NULL;
}

BillState* BillNormalShotState::HandleInput(Bill& bill, Input& input)
{
	return NULL;
}