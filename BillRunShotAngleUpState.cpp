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
}

void BillRunShotAngleUpState::Render(Bill& bill)
{
	bill.SetAnimation(BILL_ANIMATION_ID::RUN_SHOT_ANGLE_UP, bill.GetPosition(), bill.GetMovingDirection(), bill.GetAngle());
}

BillState* BillRunShotAngleUpState::Update(Bill& bill)
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

	if (bill.IsHitWall())
		bill.SetX
		(
			bill.GetX() + bill.GetVX()
		);

	return NULL;
}

BillState* BillRunShotAngleUpState::HandleInput(Bill& bill, Input& input)
{
	if (input.Is(DIK_LEFT) || input.Is(DIK_RIGHT))
	{
		if (input.Is(DIK_UP))
		{
			return NULL;
		}
	}
	return new BillNormalState();
}