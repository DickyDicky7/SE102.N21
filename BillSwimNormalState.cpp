#include "Bill.h"

BillSwimNormalState::BillSwimNormalState() : BillState()
{
}

BillSwimNormalState::~BillSwimNormalState()
{
}

void BillSwimNormalState::Exit(Bill& bill)
{
}

void BillSwimNormalState::Enter(Bill& bill)
{
}

void BillSwimNormalState::Render(Bill& bill)
{
	bill.SetAnimation(BILL_ANIMATION_ID::SWIM_NORMAL, bill.GetPosition(), bill.GetMovingDirection(), bill.GetAngle());
}

BillState* BillSwimNormalState::Update(Bill& bill)
{
	return NULL;
}

BillState* BillSwimNormalState::HandleInput(Bill& bill, Input& input)
{
	if (input.IsKey(DIK_UP) && input.IsKey(DIK_X)) return new BillSwimShotStraightUpState();
	if (                       input.IsKey(DIK_X)) return new BillSwimNormalShotState();
	if (input.IsKey(DIK_DOWN)                    ) return new BillDiveState();

	if (input.IsKey(DIK_LEFT))
	{
		bill.SetMovingDirection(DIRECTION::LEFT); return new BillSwimRunState();
	}
	if (input.IsKey(DIK_RIGHT))
	{
		bill.SetMovingDirection(DIRECTION::RIGHT); return new BillSwimRunState();
	}


	// REFACTOR LATER
	if (input.IsKey(DIK_N)) return new BillNormalState();
	//


	return NULL;
}