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
	if (bill.GetMovingDirection() == DIRECTION::LEFT)
	{
		bill.SetVX(+2.00f);
		bill.SetAX(+0.01f);
	}
	if (bill.GetMovingDirection() == DIRECTION::RIGHT)
	{
		bill.SetVX(-2.00f);
		bill.SetAX(-0.01f);
	}
}

void BillRunState::Render(Bill& bill)
{
	bill.SetAnimation(BILL_ANIMATION_ID::RUN, bill.GetPosition(), bill.GetMovingDirection(), bill.GetAngle());
}

BillState* BillRunState::Update(Bill& bill)
{
	if (bill.GetMovingDirection() == DIRECTION::LEFT)
	{
		bill.SetVX(-abs(bill.GetVX()));
		bill.SetAX(-abs(bill.GetAX()));
	}
	if (bill.GetMovingDirection() == DIRECTION::RIGHT)
	{
		bill.SetVX(+abs(bill.GetVX()));
		bill.SetAX(+abs(bill.GetAX()));
	}

	bill.SetX
	(
		// x = x0 + v0*t + a*(t^2)/2 -- uniform accelerated motion
		bill.GetX() + bill.GetVX() * time + bill.GetAX() * pow(time, 2) / 2
	);

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
	if (input.IsKey(DIK_LEFT) || input.IsKey(DIK_RIGHT))
	{
		if (input.IsKey(DIK_Z))
		{
			return new BillJumpState();
		}
		if (input.IsKey(DIK_UP))
		{
			return new BillRunShotAngleUpState();
		}
		if (input.IsKey(DIK_DOWN))
		{
			return new BillRunShotAngleDownState();
		}
		return NULL;
	}
	return new BillNormalState();
}
