#include "Bill.h"

BillRunShotAngleUpState::BillRunShotAngleUpState() : BillState()
{
}

BillRunShotAngleUpState::~BillRunShotAngleUpState()
{
}

void BillRunShotAngleUpState::Exit(Bill& bill)
{
}

void BillRunShotAngleUpState::Enter(Bill& bill)
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

void BillRunShotAngleUpState::Render(Bill& bill)
{
	bill.SetAnimation(BILL_ANIMATION_ID::RUN_SHOT_ANGLE_UP, bill.GetPosition(), bill.GetMovingDirection(), bill.GetAngle());
}

BillState* BillRunShotAngleUpState::Update(Bill& bill)
{
	auto result = Motion::CalculateUniformMotion({ bill.GetX(), bill.GetVX() });
	bill.SetX(result.c);

	return NULL;
}

BillState* BillRunShotAngleUpState::HandleInput(Bill& bill, Input& input)
{
	if (input.IsKey(DIK_LEFT )
	||  input.IsKey(DIK_RIGHT))
	{
		if (input.IsKey(DIK_UP))
		{
			if (input.IsKey(DIK_X)) bill.Fire();
			return NULL;
		}
	}
	return new BillRunState();
}