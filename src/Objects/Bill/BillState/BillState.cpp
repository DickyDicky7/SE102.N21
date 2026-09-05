#include "Bill.h"

BillState::BillState() : State()
{
	this->_time = 0.0f;
}

BillState::~BillState()
{
}

std::optional<BulletSpawnParams> BillState::GetBulletSpawnParams(const Bill&) const
{
	return std::nullopt;
}

std::optional<BulletSpawnParams> BillJumpState::GetBulletSpawnParams(const Bill& bill) const
{
	const float dirSign = (bill.GetMovingDirection() == DIRECTION::RIGHT ? 1.0f : -1.0f);
	return BulletSpawnParams{ bill.GetPosition().x + dirSign * (bill.GetW() * 0.5f), bill.GetPosition().y + bill.GetH() * Constants::Bill::GUN_SPAWN_OFFSET_RATIO_JUMP_Y, 0.0f, dirSign * Constants::Bill::BULLET_SPEED_X, 0.0f, 0.0f, 0.0f, bill.GetMovingDirection() };
}

std::optional<BulletSpawnParams> BillLayDownState::GetBulletSpawnParams(const Bill& bill) const
{
	const float dirSign = (bill.GetMovingDirection() == DIRECTION::RIGHT ? 1.0f : -1.0f);
	return BulletSpawnParams{ bill.GetPosition().x + dirSign * (bill.GetW() * 0.5f), bill.GetPosition().y + bill.GetH() * Constants::Bill::GUN_SPAWN_OFFSET_RATIO_LAYDOWN_Y, 0.0f, dirSign * Constants::Bill::BULLET_SPEED_X, 0.0f, 0.0f, 0.0f, bill.GetMovingDirection() };
}

std::optional<BulletSpawnParams> BillRunShotAngleDownState::GetBulletSpawnParams(const Bill& bill) const
{
	const float dirSign = (bill.GetMovingDirection() == DIRECTION::RIGHT ? 1.0f : -1.0f);
	return BulletSpawnParams{ bill.GetPosition().x + dirSign * (bill.GetW() * 0.5f), bill.GetPosition().y + bill.GetH() * Constants::Bill::GUN_SPAWN_OFFSET_RATIO_RUN_ANGLE_DOWN_Y, 0.0f, dirSign * Constants::Bill::BULLET_SPEED_X, -Constants::Bill::BULLET_SPEED_Y_DIAGONAL, 0.0f, 0.0f, bill.GetMovingDirection() };
}

std::optional<BulletSpawnParams> BillNormalShotState::GetBulletSpawnParams(const Bill& bill) const
{
	const float dirSign = (bill.GetMovingDirection() == DIRECTION::RIGHT ? 1.0f : -1.0f);
	return BulletSpawnParams{ bill.GetPosition().x + dirSign * (bill.GetW() * 0.5f), bill.GetPosition().y + bill.GetH() * Constants::Bill::GUN_SPAWN_OFFSET_RATIO_NORMAL_SHOT_Y, 0.0f, dirSign * Constants::Bill::BULLET_SPEED_X, 0.0f, 0.0f, 0.0f, bill.GetMovingDirection() };
}

std::optional<BulletSpawnParams> BillRunShotState::GetBulletSpawnParams(const Bill& bill) const
{
	const float dirSign = (bill.GetMovingDirection() == DIRECTION::RIGHT ? 1.0f : -1.0f);
	return BulletSpawnParams{ bill.GetPosition().x + dirSign * (bill.GetW() * 0.5f), bill.GetPosition().y + bill.GetH() * Constants::Bill::GUN_SPAWN_OFFSET_RATIO_RUN_SHOT_Y, 0.0f, dirSign * Constants::Bill::BULLET_SPEED_X, 0.0f, 0.0f, 0.0f, bill.GetMovingDirection() };
}

std::optional<BulletSpawnParams> BillShotStraightUpState::GetBulletSpawnParams(const Bill& bill) const
{
	const float dirSign = (bill.GetMovingDirection() == DIRECTION::RIGHT ? 1.0f : -1.0f);
	return BulletSpawnParams{ bill.GetPosition().x + dirSign * (bill.GetW() * 0.5f * Constants::Bill::GUN_SPAWN_OFFSET_RATIO_STRAIGHT_UP_X), bill.GetPosition().y + bill.GetH(), 0.0f, 0.0f, +Constants::Bill::BULLET_SPEED_Y_UP, 0.0f, 0.0f, bill.GetMovingDirection() };
}

std::optional<BulletSpawnParams> BillSwimShotStraightUpState::GetBulletSpawnParams(const Bill& bill) const
{
	const float dirSign = (bill.GetMovingDirection() == DIRECTION::RIGHT ? 1.0f : -1.0f);
	return BulletSpawnParams{ bill.GetPosition().x + dirSign * (bill.GetW() * 0.5f * Constants::Bill::GUN_SPAWN_OFFSET_RATIO_STRAIGHT_UP_X), bill.GetPosition().y + bill.GetH(), 0.0f, 0.0f, +Constants::Bill::BULLET_SPEED_Y_UP, 0.0f, 0.0f, bill.GetMovingDirection() };
}

std::optional<BulletSpawnParams> BillRunShotAngleUpState::GetBulletSpawnParams(const Bill& bill) const
{
	const float dirSign = (bill.GetMovingDirection() == DIRECTION::RIGHT ? 1.0f : -1.0f);
	return BulletSpawnParams{ bill.GetPosition().x + dirSign * (bill.GetW() * 0.5f), bill.GetPosition().y + bill.GetH() * Constants::Bill::GUN_SPAWN_OFFSET_RATIO_RUN_ANGLE_UP_Y, 0.0f, dirSign * Constants::Bill::BULLET_SPEED_X, +Constants::Bill::BULLET_SPEED_Y_DIAGONAL, 0.0f, 0.0f, bill.GetMovingDirection() };
}

std::optional<BulletSpawnParams> BillSwimShotAngleUpState::GetBulletSpawnParams(const Bill& bill) const
{
	const float dirSign = (bill.GetMovingDirection() == DIRECTION::RIGHT ? 1.0f : -1.0f);
	return BulletSpawnParams{ bill.GetPosition().x + dirSign * (bill.GetW() * 0.5f), bill.GetPosition().y + bill.GetH() * Constants::Bill::GUN_SPAWN_OFFSET_RATIO_SWIM_ANGLE_UP_Y, 0.0f, dirSign * Constants::Bill::BULLET_SPEED_X, +Constants::Bill::BULLET_SPEED_Y_DIAGONAL, 0.0f, 0.0f, bill.GetMovingDirection() };
}

std::optional<BulletSpawnParams> BillSwimNormalShotState::GetBulletSpawnParams(const Bill& bill) const
{
	const float dirSign = (bill.GetMovingDirection() == DIRECTION::RIGHT ? 1.0f : -1.0f);
	return BulletSpawnParams{ bill.GetPosition().x + dirSign * (bill.GetW() * 0.5f), bill.GetPosition().y + bill.GetH() * Constants::Bill::GUN_SPAWN_OFFSET_RATIO_SWIM_NORMAL_SHOT_Y, 0.0f, dirSign * Constants::Bill::BULLET_SPEED_X, 0.0f, 0.0f, 0.0f, bill.GetMovingDirection() };
}

std::optional<BulletSpawnParams> BillSwimRunShotState::GetBulletSpawnParams(const Bill& bill) const
{
	const float dirSign = (bill.GetMovingDirection() == DIRECTION::RIGHT ? 1.0f : -1.0f);
	return BulletSpawnParams{ bill.GetPosition().x + dirSign * (bill.GetW() * 0.5f), bill.GetPosition().y + bill.GetH() * Constants::Bill::GUN_SPAWN_OFFSET_RATIO_SWIM_RUN_SHOT_Y, 0.0f, dirSign * Constants::Bill::BULLET_SPEED_X, 0.0f, 0.0f, 0.0f, bill.GetMovingDirection() };
}
