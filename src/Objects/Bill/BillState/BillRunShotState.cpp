#include "Bill.h"

BillRunShotState::BillRunShotState() : BillState()
{
}

BillRunShotState::~BillRunShotState()
{
}

void BillRunShotState::Exit(Bill& bill)
{
}

void BillRunShotState::Enter(Bill& bill)
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

void BillRunShotState::Render(Bill& bill)
{
	bill.SetAnimation(BILL_ANIMATION_ID::RUN_SHOT, bill.GetPosition(), bill.GetMovingDirection(), bill.GetAngle());
}

BillState* BillRunShotState::Update(Bill& bill)
{
	auto result = Motion::CalculateUniformMotion({ bill.GetX(), bill.GetVX() });
	bill.SetX(result.coordinate);

	return nullptr;
}

BillState* BillRunShotState::HandleInput(Bill& bill, Input& input)
{
	if ((input.IsKey(DIK_LEFT) || input.IsKey(DIK_RIGHT)) && input.IsKey(DIK_X))
	{
		bill.Fire();
		return nullptr;
	}
	return new BillRunState();
}
