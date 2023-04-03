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
	if (bill.GetDirection() == DIRECTION::LEFT)
	{
		bill.SetVX(+2.00f);
		bill.SetAX(+0.01f);
	}
	if (bill.GetDirection() == DIRECTION::RIGHT)
	{
		bill.SetVX(-2.00f);
		bill.SetAX(-0.01f);
	}
}

void BillRunState::Render(Bill& bill)
{
	bill.SetAnimation(BILL_ANIMATION_ID::RUN, bill.GetPosition(), bill.GetDirection());
}

BillState* BillRunState::Update(Bill& bill)
{
	if (bill.GetDirection() == DIRECTION::LEFT)
	{
		bill.SetVX(-abs(bill.GetVX()));
		bill.SetAX(-abs(bill.GetAX()));
	}
	if (bill.GetDirection() == DIRECTION::RIGHT)
	{
		bill.SetVX(+abs(bill.GetVX()));
		bill.SetAX(+abs(bill.GetAX()));
	}

	
	// check is in screen
	if (bill.IsHitWall())
	{
		bill.SetX
		(
			// x = x0 + v0*t + a*(t^2)/2 -- uniform accelerated motion
			bill.GetX() + bill.GetVX() * time + bill.GetAX() * pow(time, 2) / 2
		);
	}
	
	// Restrict Ox velocity and time. Make this part "v0*t + a*(t^2)/2" become a constant => uniform motion
	// If not, entity will move too fast. The code here is temporary
	if (abs(bill.GetVX()) < +3.00f)
	{
		bill.SetVX
		(
			bill.GetVX() + bill.GetAX() * time
		);
	}

	// Restrict Ox velocity and time. Make this part "v0*t + a*(t^2)/2" become a constant => uniform motion
	// If not, entity will move too fast. The code here is temporary
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
