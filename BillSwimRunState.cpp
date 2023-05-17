#include "Bill.h"

BillSwimRunState::BillSwimRunState() : BillState()
{
}

BillSwimRunState::~BillSwimRunState()
{
}

void BillSwimRunState::Exit(Bill& bill)
{
}

void BillSwimRunState::Enter(Bill& bill)
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

void BillSwimRunState::Render(Bill& bill)
{
	bill.SetAnimation(BILL_ANIMATION_ID::SWIM_RUN, bill.GetPosition(), bill.GetMovingDirection(), bill.GetAngle());
}

BillState* BillSwimRunState::Update(Bill& bill)
{
	auto result = Motion::CalculateUniformMotion({ bill.GetX(), bill.GetVX() });
	bill.SetX(result.c);

	return NULL;
}

BillState* BillSwimRunState::HandleInput(Bill& bill, Input& input)
{
	if (input.IsKey(DIK_LEFT )
	||  input.IsKey(DIK_RIGHT))
	{
		if (input.IsKey(DIK_UP) && input.IsKey(DIK_X))
		{
			return new BillSwimShotAngleUpState();
		}
		if (input.IsKey(DIK_X))
		{
			return new BillSwimRunShotState();
		}
		return NULL;
	}
	return new BillSwimNormalState();
}