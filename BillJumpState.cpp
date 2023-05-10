#include "Bill.h"

BillJumpState::BillJumpState() : BillState()
{
	hasMovedLeft = 0; hasMovedRight = 0;
}

BillJumpState::~BillJumpState()
{
}

void BillJumpState::Exit(Bill& bill)
{
}

void BillJumpState::Enter(Bill& bill)
{
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

	bill.SetVY(+4.00f);
	bill.SetAY(-0.10f);
}

void BillJumpState::Render(Bill& bill)
{
	bill.SetAnimation(BILL_ANIMATION_ID::JUMP, bill.GetPosition(), bill.GetMovingDirection(), bill.GetAngle());
}

BillState* BillJumpState::Update(Bill& bill)
{
	if (bill.GetMovingDirection() == DIRECTION::LEFT)
	{
		bill.SetVX(-abs(bill.GetVX()));
		bill.SetAX(-abs(bill.GetAX()));
	}
	if (bill.GetMovingDirection() == DIRECTION::RIGHT)
	{
		bill.SetVX(+abs(bill.GetVX()));
		bill.SetAX(+abs(bill.GetAX()));
	}

	if (hasMovedLeft || hasMovedRight)
	{
		auto result = Motion::CalculateUniformMotion({ bill.GetX(), bill.GetVX() });
		bill.SetX(result.c);
	}

	if (bill.GetVY() >= 0.0f)
	{
		auto   result = Motion::CalculateUniformlyDeceleratedMotion({ bill.GetY(), bill.GetVY(), bill.GetAY(), time, 0.05f });
		time = result.t; bill.SetY(result.c)->SetVY(result.v);
	}
	if (bill.GetVY() <= 0.0f)
	{
		auto   result = Motion::CalculateUniformlyAcceleratedMotion({ bill.GetY(), bill.GetVY(), bill.GetAY(), time, 0.05f });
		time = result.t; bill.SetY(result.c)->SetVY(result.v);
	}

	if (bill.GetVY() <= 0.0f && bill.GetY() <= 0.0f)
	{
		bill.SetY(0.0f);
		return new BillNormalState();
	}

	return NULL;
}

BillState* BillJumpState::HandleInput(Bill& bill, Input& input)
{
	if (input.IsKey(DIK_LEFT))
	{
		hasMovedLeft = 1; bill.SetMovingDirection(DIRECTION::LEFT);
	}
	if (input.IsKey(DIK_RIGHT))
	{
		hasMovedRight = 1; bill.SetMovingDirection(DIRECTION::RIGHT);
	}

	return NULL;
}