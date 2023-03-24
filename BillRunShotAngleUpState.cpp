#include "Bill.h"

BillRunShotAngleUpState::BillRunShotAngleUpState(DIRECTION direction) : BillState(direction)
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
	bill.SetAnimation(BILL_RUN_SHOT_ANGLE_UP, bill.GetPosition(), direction);
}

BillState* BillRunShotAngleUpState::Update(Bill& bill)
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
	return new BillNormalState(direction);
}