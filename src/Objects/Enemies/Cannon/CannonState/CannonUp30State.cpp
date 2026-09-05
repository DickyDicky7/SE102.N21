#include "Cannon.h"

CannonUp30State::CannonUp30State()
{

}

CannonUp30State::~CannonUp30State()
{

}

void CannonUp30State::Exit(Cannon& cannon)
{

}

void CannonUp30State::Enter(Cannon& cannon)
{
	if (this->UpdateShooting(cannon))
	{
		float vx = -1.0f;
		float tanValue = std::tan(D3DXToRadian(Constants::Enemies::Cannon::AIM_ANGLE_UP_30_DEGREES));
		float vy = 1.0f * tanValue;
		if (tanValue > 1.0f)
		{
			vx = 1.0f * vx / vy;
			vy = 1.0f;
		}
		cannon.Fire(0.0f, vx, vy, 0.0f, 0.0f, cannon.GetMovingDirection());
	}
}

void CannonUp30State::Render(Cannon& cannon)
{
	cannon.SetAnimation(CANNON_ANIMATION_ID::UP_30, cannon.GetPosition(), cannon.GetMovingDirection(), cannon.GetAngle());
}

CannonState* CannonUp30State::Update(Cannon& cannon)
{
	float shootingAngle = cannon.CalculateShootingAngle();

	bool isInRange = cannon.IsTargetInRange();

	if (shootingAngle <= Constants::Enemies::Cannon::AIM_ANGLE_THRESHOLD_45_DEGREES && isInRange)
		return new CannonUp60State();

	if (shootingAngle > Constants::Enemies::Cannon::AIM_ANGLE_THRESHOLD_75_DEGREES && isInRange)
		return new CannonNormalState();

	return nullptr;
}
