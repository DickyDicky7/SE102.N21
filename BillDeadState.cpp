#include "Bill.h"

BillDeadState::BillDeadState() : BillState(), revivalCooldown(DEFAULT_REVIVAL_COOLDOWN)
{
}

BillDeadState::~BillDeadState()
{
}

void BillDeadState::Exit(Bill& bill)
{
}

void BillDeadState::Enter(Bill& bill)
{
	bill.SetVX(-bill.GetVX());
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
		auto result = Motion::CalculateUniformMotion({ bill.GetX(), bill.GetVX() });
		bill.SetX(result.c);

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
			return new BillBeginState();
		}
	}
	return NULL;
}

BillState* BillDeadState::HandleInput(Bill& bill, Input& input)
{
	return NULL;
}