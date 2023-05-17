#include "Bill.h"

BillRunShotAngleDownState::BillRunShotAngleDownState() : BillState()
{
}

BillRunShotAngleDownState::~BillRunShotAngleDownState()
{
}

void BillRunShotAngleDownState::Exit(Bill& bill)
{
}

void BillRunShotAngleDownState::Enter(Bill& bill)
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

void BillRunShotAngleDownState::Render(Bill& bill)
{
	bill.SetAnimation(BILL_ANIMATION_ID::RUN_SHOT_ANGLE_DOWN, bill.GetPosition(), bill.GetMovingDirection(), bill.GetAngle());
}

BillState* BillRunShotAngleDownState::Update(Bill& bill)
{
	auto result = Motion::CalculateUniformMotion({ bill.GetX(), bill.GetVX() });
	bill.SetX(result.c);

	return NULL;
}

BillState* BillRunShotAngleDownState::HandleInput(Bill& bill, Input& input)
{
	if (input.IsKey(DIK_LEFT )
	||  input.IsKey(DIK_RIGHT))
	{
		if (input.IsKey(DIK_DOWN))
		{
			if (input.IsKey(DIK_X)) bill.Fire();
			return NULL;
		}
	}
	return new BillRunState();
}