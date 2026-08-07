#include "Bill.h"

BillSwimShotAngleUpState::BillSwimShotAngleUpState() : BillState()
{
}

BillSwimShotAngleUpState::~BillSwimShotAngleUpState()
{
}

void BillSwimShotAngleUpState::Exit(Bill& bill)
{
}

void BillSwimShotAngleUpState::Enter(Bill& bill)
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

void BillSwimShotAngleUpState::Render(Bill& bill)
{
	bill.SetAnimation(BILL_ANIMATION_ID::SWIM_SHOT_ANGLE_UP, bill.GetPosition(), bill.GetMovingDirection(), bill.GetAngle());
}

BillState* BillSwimShotAngleUpState::Update(Bill& bill)
{
	auto result = Motion::CalculateUniformMotion({ bill.GetX(), bill.GetVX() });
	bill.SetX(result.c);

	return NULL;
}

BillState* BillSwimShotAngleUpState::HandleInput(Bill& bill, Input& input)
{
	if ((input.IsKey(DIK_LEFT) || input.IsKey(DIK_RIGHT)) && input.IsKey(DIK_UP) && input.IsKey(DIK_X))
	{
		bill.Fire();
		return NULL;
	}
	return new BillSwimRunState();
}