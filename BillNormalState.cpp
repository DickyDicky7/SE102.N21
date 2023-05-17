#include "Bill.h"

BillNormalState::BillNormalState() : BillState()
{
}

BillNormalState::~BillNormalState()
{
}

void BillNormalState::Exit(Bill& bill)
{
}

void BillNormalState::Enter(Bill& bill)
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

void BillNormalState::Render(Bill& bill)
{
	bill.SetAnimation(BILL_ANIMATION_ID::NORMAL, bill.GetPosition(), bill.GetMovingDirection(), bill.GetAngle());
}

BillState* BillNormalState::Update(Bill& bill)
{
	return NULL;
}

BillState* BillNormalState::HandleInput(Bill& bill, Input& input)
{
	if (input.IsKey(DIK_Z   )) return new BillJumpState();
	if (input.IsKey(DIK_DOWN)) return new BillLayDownState();
	if (input.IsKey(DIK_X   )) return new BillNormalShotState();
	if (input.IsKey(DIK_UP  )) return new BillStraightUpState();

	if (input.IsKey(DIK_LEFT))
	{
		bill.SetMovingDirection(DIRECTION::LEFT); return new BillRunState();
	}
	if (input.IsKey(DIK_RIGHT))
	{
		bill.SetMovingDirection(DIRECTION::RIGHT); return new BillRunState();
	}


	if (input.IsKey(DIK_F))
	{
		return new BillFallState(new BillBeginSwimState());
	}


	// FOR DEBUG PURPOSE
	if (input.IsKey(DIK_W))
	{
		bill.SetVY(+1.0f);
		bill.SetY(bill.GetY() + bill.GetVY());

	}
	if (input.IsKey(DIK_S))
	{
		bill.SetVY(-1.0f);
		bill.SetY(bill.GetY() + bill.GetVY());
	}
	if (input.IsKey(DIK_R))
	{
		bill.SetY(500);
	}
	if (input.IsKey(DIK_G))
	{
		bill.SetVX(-1.0f);
		bill.SetX(bill.GetX() + bill.GetVX());
	}
	if (input.IsKey(DIK_H))
	{
		bill.SetVX(+1.0f);
		bill.SetX(bill.GetX() + bill.GetVX());
	}
	// FOR DEBUG PURPOSE


	return NULL;
}