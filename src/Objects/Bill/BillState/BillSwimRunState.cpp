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
		bill.SetVX(-std::abs(bill.GetVX()));
		bill.SetAX(-std::abs(bill.GetAX()));
	}
	if (bill.GetMovingDirection() == DIRECTION::RIGHT)
	{
		bill.SetVX(+std::abs(bill.GetVX()));
		bill.SetAX(+std::abs(bill.GetAX()));
	}
}

void BillSwimRunState::Render(Bill& bill)
{
	bill.SetAnimation(BILL_ANIMATION_ID::SWIM_RUN, bill.GetPosition(), bill.GetMovingDirection(), bill.GetAngle());
}

BillState* BillSwimRunState::Update(Bill& bill)
{
	auto result = Motion::CalculateUniformMotion({ bill.GetX(), bill.GetVX() });
	bill.SetX(result.coordinate);

	return nullptr;
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
		return nullptr;
	}
	return new BillSwimNormalState();
}
