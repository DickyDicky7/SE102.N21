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
	bill.SetVX(-bill.GetVX());
}

void BillDeadState::Render(Bill& bill)
{
	bill.SetAnimation(BILL_ANIMATION_ID::DEAD, bill.GetPosition(), bill.GetMovingDirection(), bill.GetAngle());
}

BillState* BillDeadState::Update(Bill& bill)
{
	auto result = Motion::CalculateUniformMotion({ bill.GetX(), bill.GetVX() });
	bill.SetX(result.c);

	if (bill.GetCurrentFrame() == 3) return new BillNormalState();

	return NULL;
}

BillState* BillDeadState::HandleInput(Bill& bill, Input& input)
{
	return NULL;
}