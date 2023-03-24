#include "Bill.h"

BillJumpState::BillJumpState(DIRECTION direction) : BillState(direction)
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
	if (direction == LEFT)
	{
		bill.SetVX(+2.0f);
		bill.SetAX(+0.0f);
	}
	if (direction == RIGHT)
	{
		bill.SetVX(-2.0f);
		bill.SetAX(-0.0f);
	}

	bill.SetVY(-3.50f);
	bill.SetAY(+0.05f);
}

void BillJumpState::Render(Bill& bill)
{
	bill.SetAnimation(BILL_JUMP, bill.GetPosition(), direction);
}

BillState* BillJumpState::Update(Bill& bill)
{
	if (direction == LEFT)
	{
		bill.SetVX(-abs(bill.GetVX()));
		bill.SetAX(-abs(bill.GetAX()));
	}
	if (direction == RIGHT)
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
		return new BillNormalState(direction);
	}

	return NULL;
}

BillState* BillJumpState::HandleInput(Bill& bill, Input& input)
{
	if (input.Is(DIK_LEFT))
	{
		direction = LEFT;
		hasMovedLeft = 1;
	}
	if (input.Is(DIK_RIGHT))
	{
		direction = RIGHT;
		hasMovedRight = 1;
	}

	return NULL;
}