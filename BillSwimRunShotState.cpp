#include "Bill.h"

BillSwimRunShotState::BillSwimRunShotState() : BillState()
{
}

BillSwimRunShotState::~BillSwimRunShotState()
{
}

void BillSwimRunShotState::Exit(Bill& bill)
{
}

void BillSwimRunShotState::Enter(Bill& bill)
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

void BillSwimRunShotState::Render(Bill& bill)
{
	bill.SetAnimation(BILL_ANIMATION_ID::SWIM_SHOT, bill.GetPosition(), bill.GetMovingDirection(), bill.GetAngle());
}

BillState* BillSwimRunShotState::Update(Bill& bill)
{
	auto result = Motion::CalculateUniformMotion({ bill.GetX(), bill.GetVX() });
	bill.SetX(result.c);

	return NULL;
}

BillState* BillSwimRunShotState::HandleInput(Bill& bill, Input& input)
{
	if ((input.IsKey(DIK_LEFT) || input.IsKey(DIK_RIGHT)) && input.IsKey(DIK_X))
	{
		bill.Fire();
		return NULL;
	}
	return new BillSwimRunState();
}