#include "Bill.h"

BillBeginSwimState::BillBeginSwimState() : BillState()
{
	Sound::GetInstance()->Play("landing", false, 1);
	this->_delayTime = Constants::Bill::SWIM_BEGIN_DELAY_FRAMES;
}

BillBeginSwimState::~BillBeginSwimState()
{
}

void BillBeginSwimState::Exit(Bill& bill)
{
}

void BillBeginSwimState::Enter(Bill& bill)
{
	if (bill.GetMovingDirection() == DIRECTION::LEFT)
	{
		bill.SetVX(-Constants::Bill::SWIM_SPEED_X);
		bill.SetAX(-0.0f);
	}
	if (bill.GetMovingDirection() == DIRECTION::RIGHT)
	{
		bill.SetVX(+Constants::Bill::SWIM_SPEED_X);
		bill.SetAX(+0.0f);
	}

	bill.SetVY(+Constants::Bill::SWIM_BUOYANCY_VELOCITY_Y);
	bill.SetAY(+Constants::Bill::SWIM_BUOYANCY_ACCELERATION_Y);
}

void BillBeginSwimState::Render(Bill& bill)
{
	bill.SetAnimation(BILL_ANIMATION_ID::BEGIN_SWIM, bill.GetPosition(), bill.GetMovingDirection(), bill.GetAngle());
}

BillState* BillBeginSwimState::Update(Bill& bill)
{
	if (--this->_delayTime == 0) return new BillSwimNormalState();
	return nullptr;
}

BillState* BillBeginSwimState::HandleInput(Bill& bill, Input& input)
{
	return nullptr;
}
