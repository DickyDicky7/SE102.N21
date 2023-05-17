#include "Bill.h"

BillSwimNormalShotState::BillSwimNormalShotState() : BillState()
{
}

BillSwimNormalShotState::~BillSwimNormalShotState()
{
}

void BillSwimNormalShotState::Exit(Bill& bill)
{
}

void BillSwimNormalShotState::Enter(Bill& bill)
{
}

void BillSwimNormalShotState::Render(Bill& bill)
{
	bill.SetAnimation(BILL_ANIMATION_ID::SWIM_SHOT, bill.GetPosition(), bill.GetMovingDirection(), bill.GetAngle());
}

BillState* BillSwimNormalShotState::Update(Bill& bill)
{
	return NULL;
}

BillState* BillSwimNormalShotState::HandleInput(Bill& bill, Input& input)
{
	if (input.IsKey(DIK_X))
	{
		bill.Fire();
		return NULL;
	}
	return new BillSwimNormalState();
}