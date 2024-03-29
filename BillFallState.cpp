#include "Bill.h"

BillFallState::BillFallState() : BillState()
{
	this->returnState = NULL;
}

BillFallState::BillFallState(BillState* returnState) : BillState()
{
	this->returnState = returnState;
}

BillFallState::~BillFallState()
{
}

void BillFallState::Exit(Bill& bill)
{
}

void BillFallState::Enter(Bill& bill)
{
	if (returnState)
	bill.SetY (+std::numeric_limits<FLOAT>::infinity());
	bill.SetVY(-2.50f);
	bill.SetAY(-0.10f);
}

void BillFallState::Render(Bill& bill)
{
	bill.SetAnimation(BILL_ANIMATION_ID::FALL, bill.GetPosition(), bill.GetMovingDirection(), bill.GetAngle());
}

BillState* BillFallState::Update(Bill& bill)
{
	auto result = Motion::CalculateUniformlyAcceleratedMotion({ bill.GetY(), bill.GetVY(), bill.GetAY(), time, 0.05f });

	time = result.t;
	bill.SetY(result.c); bill.SetVY(result.v);

	if (bill.GetVY() <= 0.0f && bill.GetY() <= 0.0f)
	{
		bill.SetY(0.0f);
		return returnState;
	}

	return NULL;
}

BillState* BillFallState::HandleInput(Bill& bill, Input& input)
{
	return NULL;
}