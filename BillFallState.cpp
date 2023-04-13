#include "Bill.h"

BillFallState::BillFallState() : BillState()
{
}

BillFallState::~BillFallState()
{
}

void BillFallState::Exit(Bill& bill)
{
}

void BillFallState::Enter(Bill& bill)
{
	bill.SetY(300);
	bill.SetVY(-2.50f);
	bill.SetAY(-0.10f);
}

void BillFallState::Render(Bill& bill)
{
	bill.SetAnimation(BILL_ANIMATION_ID::FALL, bill.GetPosition(), bill.GetMovingDirection(), bill.GetAngle());
}

BillState* BillFallState::Update(Bill& bill)
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
		return new BillBeginSwimState();
	}

	return NULL;
}

BillState* BillFallState::HandleInput(Bill& bill, Input& input)
{
	return NULL;
}