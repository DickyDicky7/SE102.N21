#include "Bill.h"

BillSwimShotAngleUpState::BillSwimShotAngleUpState() : BillState()
{
}

BillSwimShotAngleUpState::~BillSwimShotAngleUpState()
{
}

void BillSwimShotAngleUpState::Exit(Bill& bill)
{
}

void BillSwimShotAngleUpState::Enter(Bill& bill)
{
}

void BillSwimShotAngleUpState::Render(Bill& bill)
{
	bill.SetAnimation(BILL_ANIMATION_ID::SWIM_SHOT_ANGLE_UP, bill.GetPosition(), bill.GetMovingDirection(), bill.GetAngle());
}

BillState* BillSwimShotAngleUpState::Update(Bill& bill)
{
	return NULL;
}

BillState* BillSwimShotAngleUpState::HandleInput(Bill& bill, Input& input)
{
	return NULL;
}