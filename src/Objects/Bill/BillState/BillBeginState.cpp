#include "Bill.h"

BillBeginState::BillBeginState() : BillState()
{
}

BillBeginState::~BillBeginState()
{
}

void BillBeginState::Exit(Bill& bill)
{
}

void BillBeginState::Enter(Bill& bill)
{
	bill.SetY (+std::numeric_limits<float>::infinity());
	bill.SetVY(Constants::Bill::FALL_SPEED_Y);
	bill.SetAY(Constants::Bill::FALL_ACCELERATION_Y);
	bill.ResetImmortalTick();
}

void BillBeginState::Render(Bill& bill)
{
	bill.SetAnimation(BILL_ANIMATION_ID::BEGIN, bill.GetPosition(), bill.GetMovingDirection(), bill.GetAngle());
}

BillState* BillBeginState::Update(Bill& bill)
{
	auto result = Motion::CalculateUniformlyAcceleratedMotion({ bill.GetY(), bill.GetVY(), bill.GetAY(), this->_time, Constants::Physics::DEFAULT_MOTION_INTEGRATION_DELTA_TIME });

	this->_time = result.elapsedTime;
	bill.SetY(result.coordinate); bill.SetVY(result.velocity);

	if (bill.GetVY() <= 0.0f && bill.GetY() <= 0.0f)
	{
		bill.SetY(0.0f);
		return new BillNormalState();
	}

	return nullptr;
}

BillState* BillBeginState::HandleInput(Bill& bill, Input& input)
{
	return nullptr;
}
