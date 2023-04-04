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
	bill.SetVY(+2.50f);
	bill.SetAY(+0.10f);

	bill.SetY(SCREEN_HEIGHT / 2 - 50 - 200);
}

void BillBeginState::Render(Bill& bill)
{
	bill.SetAnimation(BILL_ANIMATION_ID::BEGIN, bill.GetPosition(), bill.GetMovingDirection());
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

	if (bill.GetVY() >= 0 && bill.GetY() >= SCREEN_HEIGHT / 2 - 50)
	{
		bill.SetY(SCREEN_HEIGHT / 2 - 50);
		return new BillNormalState();
	}

	return NULL;
}

BillState* BillBeginState::HandleInput(Bill& bill, Input& input)
{
	return NULL;
}