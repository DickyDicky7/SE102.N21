#include "Bill.h"

BillRunState::BillRunState() : BillState()
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
	if (bill.GetDirection() == LEFT)
	{
		bill.SetVX(+2.00f);
		bill.SetAX(+0.01f);
	}
	if (bill.GetDirection() == RIGHT)
	{
		bill.SetVX(-2.00f);
		bill.SetAX(-0.01f);
	}
}

void BillRunState::Render(Bill& bill)
{
	bill.SetAnimation(BILL_RUN, bill.GetPosition(), bill.GetDirection());
}

BillState* BillRunState::Update(Bill& bill)
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
			return new BillJumpState();
		}
		if (input.Is(DIK_UP))
		{
			return new BillRunShotAngleUpState();
		}
		if (input.Is(DIK_DOWN))
		{
			return new BillRunShotAngleDownState();
		}
		return NULL;
	}
	return new BillNormalState();
}