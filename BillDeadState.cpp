#include "Bill.h"

BillDeadState::BillDeadState() : BillState(), revivalCooldown(DEFAULT_REVIVAL_COOLDOWN)
{
	Sound::getInstance()->play("playerdie", false, 1);
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
		bill.SetVX(+1.0f);
	}
	else
	if (bill.GetMovingDirection() == DIRECTION::RIGHT)
	{
		bill.SetVX(-1.0f);
	}

	bill.SetVY(+2.5f);
	bill.SetAY(-0.1f);
}

void BillDeadState::Render(Bill& bill)
{
	if (!bill.isDead)
		 bill.SetAnimation(BILL_ANIMATION_ID::DEAD, bill.GetPosition(), bill.GetMovingDirection(), bill.GetAngle());
}

BillState* BillDeadState::Update(Bill& bill)
{
	if (*bill.livesLeft <= -1)
	{
		return NULL;
	}
	if (!bill.isDead)
	{
		auto resultX = Motion::CalculateUniformMotion({ bill.GetX(), bill.GetVX() });
		bill.SetX(resultX.c);

		if (bill.GetVY() >= 0.0f)
		{
			auto   resultY = Motion::CalculateUniformlyDeceleratedMotion({ bill.GetY(), bill.GetVY(), bill.GetAY(), time, 0.05f });
			time = resultY.t; bill.SetY(resultY.c); bill.SetVY(resultY.v);
		}
		if (bill.GetVY() <= 0.0f)
		{
			auto   resultY = Motion::CalculateUniformlyAcceleratedMotion({ bill.GetY(), bill.GetVY(), bill.GetAY(), time, 0.05f });
			time = resultY.t; bill.SetY(resultY.c); bill.SetVY(resultY.v);
		}

		if (bill.GetCurrentFrame() == 3)
		{
			bill.isDead = 1;
		}
	}
	else
	{
		if (revivalCooldown == DEFAULT_REVIVAL_COOLDOWN) --(*bill.livesLeft);
		  --revivalCooldown;
		if (revivalCooldown == 0)
		{
			revivalCooldown  = DEFAULT_REVIVAL_COOLDOWN;
			bill.isDead      = 0;
			bill.SetBulletState(new BulletRState());
			return new BillBeginState();
		}
	}
	return NULL;
}

BillState* BillDeadState::HandleInput(Bill& bill, Input& input)
{
	return NULL;
}