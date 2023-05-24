#include "Bill.h"

BillBeginSwimState::BillBeginSwimState() : BillState()
{
	delayTime = 50;
}

BillBeginSwimState::~BillBeginSwimState()
{
}

void BillBeginSwimState::Exit(Bill& bill)
{
}

void BillBeginSwimState::Enter(Bill& bill)
{
	//bill.SetVX(0.0f);
	//bill.SetVY(0.0f);
	//bill.SetAX(0.0f);
	//bill.SetAY(0.0f);

	if (bill.GetMovingDirection() == DIRECTION::LEFT)
	{
		bill.SetVX(-2.0f);
		bill.SetAX(-0.0f);
	}
	if (bill.GetMovingDirection() == DIRECTION::RIGHT)
	{
		bill.SetVX(+2.0f);
		bill.SetAX(+0.0f);
	}

	bill.SetVY(0.0f);
	bill.SetAY(0.0f);
}

void BillBeginSwimState::Render(Bill& bill)
{
	bill.SetAnimation(BILL_ANIMATION_ID::BEGIN_SWIM, bill.GetPosition(), bill.GetMovingDirection(), bill.GetAngle());
}

BillState* BillBeginSwimState::Update(Bill& bill)
{
	if (--delayTime == 0) return new BillSwimNormalState();
	return NULL;
}

BillState* BillBeginSwimState::HandleInput(Bill& bill, Input& input)
{
	return NULL;
}