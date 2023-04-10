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
	bill.SetY
	(
		bill.GetY() + bill.GetVY() * time + bill.GetAY() * pow(time, 2) / 2
	);
	bill.SetVY
	(
		bill.GetVY() + bill.GetAY() * time
	);

	time += 0.05f;

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