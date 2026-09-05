#include "Bill.h"

BillJumpState::BillJumpState() : BillState()
{
	this->_hasMovedLeft = false; this->_hasMovedRight = false;
}

BillJumpState::~BillJumpState()
{
}

void BillJumpState::Exit(Bill& bill)
{
	Sound::GetInstance()->Play("landing", false, 1);
}

void BillJumpState::Enter(Bill& bill)
{
	if (bill.GetMovingDirection() == DIRECTION::LEFT)
	{
		bill.SetVX(-Constants::Bill::JUMP_SPEED_X);
		bill.SetAX(-0.0f);
	}
	if (bill.GetMovingDirection() == DIRECTION::RIGHT)
	{
		bill.SetVX(+Constants::Bill::JUMP_SPEED_X);
		bill.SetAX(+0.0f);
	}

	bill.SetVY(+Constants::Bill::JUMP_SPEED_Y);
	bill.SetAY(Constants::Bill::JUMP_ACCELERATION_Y);
}

void BillJumpState::Render(Bill& bill)
{
	bill.SetAnimation(BILL_ANIMATION_ID::JUMP, bill.GetPosition(), bill.GetMovingDirection(), bill.GetAngle());
}

BillState* BillJumpState::Update(Bill& bill)
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

	if (this->_hasMovedLeft || this->_hasMovedRight)
	{
		auto result = Motion::CalculateUniformMotion({ bill.GetX(), bill.GetVX() });
		bill.SetX(result.coordinate);
	}

	if (bill.GetVY() >= 0.0f)
	{
		auto resultY = Motion::CalculateUniformlyDeceleratedMotion({ bill.GetY(), bill.GetVY(), bill.GetAY(), this->_time, Constants::Physics::DEFAULT_MOTION_INTEGRATION_DELTA_TIME });
		this->_time = resultY.elapsedTime;
		bill.SetY(resultY.coordinate);
		bill.SetVY(resultY.velocity);
	}
	else
	{
		auto resultY = Motion::CalculateUniformlyAcceleratedMotion({ bill.GetY(), bill.GetVY(), bill.GetAY(), this->_time, Constants::Physics::DEFAULT_MOTION_INTEGRATION_DELTA_TIME });
		this->_time = resultY.elapsedTime;
		bill.SetY(resultY.coordinate);
		bill.SetVY(resultY.velocity);
	}

	if (bill.GetVY() <= 0.0f && bill.GetY() <= 0.0f)
	{
		bill.SetY(0.0f);
		Sound::GetInstance()->Play("landing", false, 1);
		return new BillNormalState();
	}

	return nullptr;
}

BillState* BillJumpState::HandleInput(Bill& bill, Input& input)
{
	if (input.IsKey(DIK_X))
	{
		bill.Fire();
	}
	if (input.IsKey(DIK_LEFT))
	{
		this->_hasMovedLeft = true; bill.SetMovingDirection(DIRECTION::LEFT);
	}
	if (input.IsKey(DIK_RIGHT))
	{
		this->_hasMovedRight = true; bill.SetMovingDirection(DIRECTION::RIGHT);
	}

	return nullptr;
}
