#include "Bill.h"

BillFallState::BillFallState() : BillState()
{
	this->_returnState = nullptr;
}

BillFallState::BillFallState(BillState* returnState) : BillState()
{
	this->_returnState = returnState;
}

BillFallState::~BillFallState()
{
	// Owned until Update hands it back (which clears it first).  A fall almost
	// never ends through Update: landing on terrain, drowning or dying all
	// re-enter another state from the collision handlers, and each of those
	// destroys this one with the queued return state still attached.
	Destroy(this->_returnState);
}

void BillFallState::Exit(Bill& bill)
{
}

void BillFallState::Enter(Bill& bill)
{
	if (this->_returnState)
	bill.SetY (+std::numeric_limits<float>::infinity());
	bill.SetVY(Constants::Bill::FALL_SPEED_Y);
	bill.SetAY(Constants::Bill::FALL_ACCELERATION_Y);
}

void BillFallState::Render(Bill& bill)
{
	bill.SetAnimation(BILL_ANIMATION_ID::FALL, bill.GetPosition(), bill.GetMovingDirection(), bill.GetAngle());
}

BillState* BillFallState::Update(Bill& bill)
{
	auto result = Motion::CalculateUniformlyAcceleratedMotion({ bill.GetY(), bill.GetVY(), bill.GetAY(), this->_time, Constants::Physics::DEFAULT_MOTION_INTEGRATION_DELTA_TIME });

	this->_time = result.elapsedTime;
	bill.SetY(result.coordinate); bill.SetVY(result.velocity);

	if (bill.GetVY() <= 0.0f && bill.GetY() <= 0.0f)
	{
		bill.SetY(0.0f);
		BillState* next = this->_returnState;
		this->_returnState = nullptr;   // ownership passes to the caller; the destructor must not free it
		return next;
	}

	return nullptr;
}

BillState* BillFallState::HandleInput(Bill& bill, Input& input)
{
	return nullptr;
}
