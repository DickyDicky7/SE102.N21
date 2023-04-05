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
	bill.SetVY(+2.50f);
	bill.SetAY(+0.10f);

	bill.SetY(SCREEN_HEIGHT / 2 - 50 - 200);
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

	if (bill.GetVY() >= 0 && bill.GetY() >= SCREEN_HEIGHT / 2 - 50)
	{
		bill.SetY(SCREEN_HEIGHT / 2 - 50);
		return new BillBeginSwimState();
	}

	return NULL;
}

BillState* BillFallState::HandleInput(Bill& bill, Input& input)
{
	return NULL;
}