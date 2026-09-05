#include "Bill.h"

BillDeadState::BillDeadState() : BillState(), _revivalCooldown(DEFAULT_REVIVAL_COOLDOWN)
{
	Sound::GetInstance()->Play("playerdie", false, 1);
}

BillDeadState::~BillDeadState()
{
}

void BillDeadState::Exit(Bill& bill)
{
}

void BillDeadState::Enter(Bill& bill)
{
	if (bill.GetMovingDirection() == DIRECTION::LEFT)
	{
		bill.SetVX(+Constants::Bill::DEAD_SPEED_X);
	}
	else
	if (bill.GetMovingDirection() == DIRECTION::RIGHT)
	{
		bill.SetVX(-Constants::Bill::DEAD_SPEED_X);
	}

	bill.SetVY(+Constants::Bill::DEAD_SPEED_Y);
	bill.SetAY(Constants::Bill::DEAD_ACCELERATION_Y);
}

void BillDeadState::Render(Bill& bill)
{
	if (!bill.IsDead())
		 bill.SetAnimation(BILL_ANIMATION_ID::DEAD, bill.GetPosition(), bill.GetMovingDirection(), bill.GetAngle());
}

BillState* BillDeadState::Update(Bill& bill)
{
	if (*bill.GetLivesLeft() <= -1)
	{
		return nullptr;
	}
	if (!bill.IsDead())
	{
		auto resultX = Motion::CalculateUniformMotion({ bill.GetX(), bill.GetVX() });
		bill.SetX(resultX.coordinate);

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

		if (bill.GetCurrentFrame() == Constants::Bill::DEAD_FINAL_FRAME)
		{
			bill.SetDead(true);
		}
	}
	else
	{
		if (this->_revivalCooldown == DEFAULT_REVIVAL_COOLDOWN) --(*bill.GetLivesLeft());
		  --this->_revivalCooldown;
		if (this->_revivalCooldown == 0)
		{
			this->_revivalCooldown  = DEFAULT_REVIVAL_COOLDOWN;
			bill.SetDead(false);
			return new BillBeginState();
		}
	}
	return nullptr;
}

BillState* BillDeadState::HandleInput(Bill& bill, Input& input)
{
	return nullptr;
}
