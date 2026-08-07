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
		bill.SetVX(-abs(bill.GetVX()));
		bill.SetAX(-abs(bill.GetAX()));
	}
	if (bill.GetMovingDirection() == DIRECTION::RIGHT)
	{
		bill.SetVX(+abs(bill.GetVX()));
		bill.SetAX(+abs(bill.GetAX()));
	}
}

void BillRunState::Render(Bill& bill)
{
	bill.SetAnimation(BILL_ANIMATION_ID::RUN, bill.GetPosition(), bill.GetMovingDirection(), bill.GetAngle());
}

BillState* BillRunState::Update(Bill& bill)
{
	auto result = Motion::CalculateUniformMotion({ bill.GetX(), bill.GetVX() });
	bill.SetX(result.c);

	return NULL;
}

BillState* BillRunState::HandleInput(Bill& bill, Input& input)
{
	if (input.IsKey(DIK_LEFT )
	||  input.IsKey(DIK_RIGHT))
	{
		if (input.IsKey(DIK_Z))
		{
			return new BillJumpState();
		}
		if (input.IsKey(DIK_X))
		{
			return new BillRunShotState();
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