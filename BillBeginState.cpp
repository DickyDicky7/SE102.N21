#include "Bill.h"

BillBeginState::BillBeginState() : BillState()
{
}

BillBeginState::~BillBeginState()
{
}

void BillBeginState::Exit(Bill& bill)
{
}

void BillBeginState::Enter(Bill& bill)
{
	bill.SetY(300);
	bill.SetVY(-2.50f);
	bill.SetAY(-0.10f);
}

void BillBeginState::Render(Bill& bill)
{
	bill.SetAnimation(BILL_ANIMATION_ID::BEGIN, bill.GetPosition(), bill.GetMovingDirection(), bill.GetAngle());
}

BillState* BillBeginState::Update(Bill& bill)
{
	auto result = Motion::CalculateUniformlyAcceleratedMotion({ bill.GetY(), bill.GetVY(), bill.GetAY(), time, 0.05f });

	time = result.t;
	bill.SetY(result.c)->SetVY(result.v);

	if (bill.GetVY() <= 0 && bill.GetY() <= 0)
	{
		bill.SetY(0);
		return new BillNormalState();
	}

	return NULL;
}

BillState* BillBeginState::HandleInput(Bill& bill, Input& input)
{
	return NULL;
}