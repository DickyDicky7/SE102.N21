#include "Bill.h"

BillNormalState::BillNormalState(DIRECTION direction) : BillState(direction)
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
}

void BillNormalState::Render(Bill& bill)
{
	bill.SetAnimation(BILL_NORMAL, bill.GetPosition(), direction);
}

BillState* BillNormalState::Update(Bill& bill)
{
	return NULL;
}

BillState* BillNormalState::HandleInput(Bill& bill, Input& input)
{
	if (input.Is(DIK_LEFT))
	{
		OutputDebugString(L"LEFT entered\n");
		return new BillRunState(LEFT);
	}
	if (input.Is(DIK_RIGHT))
	{
		OutputDebugString(L"RIGHT entered\n");
		return new BillRunState(RIGHT);
	}
	if (input.Is(DIK_Z))
	{
		OutputDebugString(L"Z entered\n");
		return new BillJumpState(direction);
	}
	if (input.Is(DIK_DOWN))
	{
		OutputDebugString(L"DOWN entered\n");
		return new BillLayDownState(direction);
	}

	if (input.Is(DIK_B))
	{
		OutputDebugString(L"B entered\n");
		return new BillSwimState(direction);
	}

	return NULL;
}