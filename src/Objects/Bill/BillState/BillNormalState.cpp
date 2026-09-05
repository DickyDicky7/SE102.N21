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
	if (bill.GetMovingDirection() == DIRECTION::LEFT)
	{
		bill.SetVX(-Constants::Bill::MOVE_SPEED);
		bill.SetAX(-0.0f);
	}
	if (bill.GetMovingDirection() == DIRECTION::RIGHT)
	{
		bill.SetVX(+Constants::Bill::MOVE_SPEED);
		bill.SetAX(+0.0f);
	}

	bill.SetVY(+Constants::Bill::NORMAL_RESET_VELOCITY_Y);
	bill.SetAY(+Constants::Bill::NORMAL_RESET_ACCELERATION_Y);
}

void BillNormalState::Render(Bill& bill)
{
	bill.SetAnimation(BILL_ANIMATION_ID::NORMAL, bill.GetPosition(), bill.GetMovingDirection(), bill.GetAngle());
}

BillState* BillNormalState::Update(Bill& bill)
{
	return nullptr;
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

	if (input.IsKey(DIK_C))
	{
		bill.SetY(bill.GetY() - bill.GetH() * 0.5f);
		return new BillFallState();
	}

	if (input.IsKey(DIK_F))
	{
		return new BillFallState(new BillBeginSwimState());
	}

	return nullptr;
}
