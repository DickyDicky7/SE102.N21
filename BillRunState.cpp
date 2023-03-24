#include "Bill.h"

BillRunState::BillRunState(DIRECTION direction) : BillState(direction)
{
}

BillRunState::~BillRunState()
{
}

void BillRunState::Exit(Bill& bill)
{
}

void BillRunState::Enter(Bill& bill)
{
	if (direction == LEFT)
	{
		bill.SetVX(+2.00f);
		bill.SetAX(+0.01f);
	}
	if (direction == RIGHT)
	{
		bill.SetVX(-2.00f);
		bill.SetAX(-0.01f);
	}
}

void BillRunState::Render(Bill& bill)
{
	bill.SetAnimation(BILL_RUN, bill.GetPosition(), direction);
}

BillState* BillRunState::Update(Bill& bill)
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

	bill.SetX
	(
		bill.GetX() + bill.GetVX() * time + bill.GetAX() * pow(time, 2) / 2
	);

	if (abs(bill.GetVX()) < +3.00f)
	{
		bill.SetVX
		(
			bill.GetVX() + bill.GetAX() * time
		);
	}

	if (time < +2.00f)
	{
		time += 0.05f;
	}

	return NULL;
}

BillState* BillRunState::HandleInput(Bill& bill, Input& input)
{
	if (input.Is(DIK_LEFT) || input.Is(DIK_RIGHT))
	{
		if (input.Is(DIK_Z))
		{
			return new BillJumpState(direction);
		}
		if (input.Is(DIK_UP))
		{
			return new BillRunShotAngleUpState(direction);
		}
		if (input.Is(DIK_DOWN))
		{
			return new BillRunShotAngleDownState(direction);
		}
		return NULL;
	}
	return new BillNormalState(direction);
}