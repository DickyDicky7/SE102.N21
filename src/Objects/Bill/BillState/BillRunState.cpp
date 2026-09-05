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
		bill.SetVX(-std::abs(bill.GetVX()));
		bill.SetAX(-std::abs(bill.GetAX()));
	}
	if (bill.GetMovingDirection() == DIRECTION::RIGHT)
	{
		bill.SetVX(+std::abs(bill.GetVX()));
		bill.SetAX(+std::abs(bill.GetAX()));
	}
}

void BillRunState::Render(Bill& bill)
{
	bill.SetAnimation(BILL_ANIMATION_ID::RUN, bill.GetPosition(), bill.GetMovingDirection(), bill.GetAngle());
}

BillState* BillRunState::Update(Bill& bill)
{
	auto result = Motion::CalculateUniformMotion({ bill.GetX(), bill.GetVX() });
	bill.SetX(result.coordinate);

	return nullptr;
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
		return nullptr;
	}
	return new BillNormalState();
}
