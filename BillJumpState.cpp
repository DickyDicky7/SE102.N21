#include "Bill.h"

BillJumpState::BillJumpState() : BillState()
{
	hasMovedLeft = 0;
	hasMovedRight = 0;
}

BillJumpState::~BillJumpState()
{
}

void BillJumpState::Exit(Bill& bill)
{
}

void BillJumpState::Enter(Bill& bill)
{
	if (bill.GetDirection() == LEFT)
	{
		bill.SetVX(+2.0f);
		bill.SetAX(+0.0f);
	}
	if (bill.GetDirection() == RIGHT)
	{
		bill.SetVX(-2.0f);
		bill.SetAX(-0.0f);
	}

	bill.SetVY(-4.00f);
	bill.SetAY(+0.10f);
}

void BillJumpState::Render(Bill& bill)
{
	bill.SetAnimation(BILL_JUMP, bill.GetPosition(), bill.GetDirection());
}

BillState* BillJumpState::Update(Bill& bill)
{
	if (bill.GetDirection() == LEFT)
	{
		bill.SetVX(-abs(bill.GetVX()));
		bill.SetAX(-abs(bill.GetAX()));
	}
	if (bill.GetDirection() == RIGHT)
	{
		bill.SetVX(+abs(bill.GetVX()));
		bill.SetAX(+abs(bill.GetAX()));
	}
	if (hasMovedLeft || hasMovedRight)
	{
		bill.SetX
		(
			bill.GetX() + bill.GetVX()
		);
	}

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

BillState* BillJumpState::HandleInput(Bill& bill, Input& input)
{
	if (input.Is(DIK_LEFT))
	{
		hasMovedLeft = 1;
		bill.SetDirection(LEFT);
	}
	if (input.Is(DIK_RIGHT))
	{
		hasMovedRight = 1;
		bill.SetDirection(RIGHT);
	}

	return NULL;
}