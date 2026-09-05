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
		bill.SetVX(-std::abs(bill.GetVX()));
		bill.SetAX(-std::abs(bill.GetAX()));
	}
	if (bill.GetMovingDirection() == DIRECTION::RIGHT)
	{
		bill.SetVX(+std::abs(bill.GetVX()));
		bill.SetAX(+std::abs(bill.GetAX()));
	}
}

void BillRunShotAngleDownState::Render(Bill& bill)
{
	bill.SetAnimation(BILL_ANIMATION_ID::RUN_SHOT_ANGLE_DOWN, bill.GetPosition(), bill.GetMovingDirection(), bill.GetAngle());
}

BillState* BillRunShotAngleDownState::Update(Bill& bill)
{
	auto result = Motion::CalculateUniformMotion({ bill.GetX(), bill.GetVX() });
	bill.SetX(result.coordinate);

	return nullptr;
}

BillState* BillRunShotAngleDownState::HandleInput(Bill& bill, Input& input)
{
	if (input.IsKey(DIK_LEFT )
	||  input.IsKey(DIK_RIGHT))
	{
		if (input.IsKey(DIK_DOWN))
		{
			if (input.IsKey(DIK_X)) bill.Fire();
			return nullptr;
		}
	}
	return new BillRunState();
}
