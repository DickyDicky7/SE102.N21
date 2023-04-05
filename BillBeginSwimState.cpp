#include "Bill.h"

BillBeginSwimState::BillBeginSwimState() : BillState()
{
	delayTime = 50;
}

BillBeginSwimState::~BillBeginSwimState()
{
}

void BillBeginSwimState::Exit(Bill& bill)
{
}

void BillBeginSwimState::Enter(Bill& bill)
{
}

void BillBeginSwimState::Render(Bill& bill)
{
	bill.SetAnimation(BILL_ANIMATION_ID::BEGIN_SWIM, bill.GetPosition(), bill.GetMovingDirection(), bill.GetAngle());
}

BillState* BillBeginSwimState::Update(Bill& bill)
{
	if (--delayTime == 0) return new BillSwimNormalState();
	return NULL;
}

BillState* BillBeginSwimState::HandleInput(Bill& bill, Input& input)
{
	return NULL;
}