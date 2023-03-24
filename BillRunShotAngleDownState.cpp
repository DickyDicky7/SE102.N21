#include "Bill.h"

#pragma region
BillRunShotAngleDownState::BillRunShotAngleDownState(DIRECTION direction) : BillState(direction)
{
}

BillRunShotAngleDownState::~BillRunShotAngleDownState()
{
}

void BillRunShotAngleDownState::Exit(Bill& bill)
{
	bill.SetAnimation(BILL_RUN_SHOT_ANGLE_DOWN, bill.GetPosition(), direction);
}

void BillRunShotAngleDownState::Enter(Bill& bill)
{
	bill.SetAnimation(BILL_RUN_SHOT_ANGLE_DOWN, bill.GetPosition(), direction);
}

void BillRunShotAngleDownState::Render(Bill& bill)
{
	bill.SetAnimation(BILL_RUN_SHOT_ANGLE_DOWN, bill.GetPosition(), direction);
}

BillState* BillRunShotAngleDownState::Update(Bill& bill)
{
	if (direction == LEFT)
		bill.SetX(bill.GetX() - bill.GetVX());
	if (direction == RIGHT)
		bill.SetX(bill.GetX() + bill.GetVX());
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
#pragma endregion Bill Run Shot Angle Down State