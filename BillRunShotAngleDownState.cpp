#include "Bill.h"

BillRunShotAngleDownState::BillRunShotAngleDownState(DIRECTION direction) : BillState(direction)
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
}

void BillRunShotAngleDownState::Render(Bill& bill)
{
	bill.SetAnimation(BILL_RUN_SHOT_ANGLE_DOWN, bill.GetPosition(), direction);
}

BillState* BillRunShotAngleDownState::Update(Bill& bill)
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

BillState* BillRunShotAngleDownState::HandleInput(Bill& bill, Input& input)
{

	if (input.Is(DIK_LEFT) || input.Is(DIK_RIGHT))
	{
		if (input.Is(DIK_DOWN))
		{
			return NULL;
		}
	}
	return new BillNormalState(direction);
}