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

	auto result = Motion::CalculateUniformlyAcceleratedMotion({ bill.GetX(), bill.GetVX(), bill.GetAX(), time, 0.05f });

	bill.SetX(result.c);
	if (time < +2.00f) time = result.t;
	if (abs(bill.GetVX()) < +3.00f) bill.SetVX(result.v);

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
