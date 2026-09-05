#include "Cannon.h"

CannonUp60State::CannonUp60State()
{

}

CannonUp60State::~CannonUp60State()
{

}

void CannonUp60State::Exit(Cannon&)
{

}

void CannonUp60State::Enter(Cannon& cannon)
{
	if (this->UpdateShooting(cannon))
	{
		float vx = -1.0f;
		float tanValue = std::tan(D3DXToRadian(Constants::Enemies::Cannon::AIM_ANGLE_UP_60_DEGREES));
		float vy = 1.0f * tanValue;
		if (tanValue > 1.0f)
		{
			vx = 1.0f * vx / vy;
			vy = 1.0f;
		}
		cannon.Fire(0.0f, vx, vy, 0.0f, 0.0f, cannon.GetMovingDirection());
	}
}

void CannonUp60State::Render(Cannon& cannon)
{
	cannon.SetAnimation(CANNON_ANIMATION_ID::UP_60, cannon.GetPosition(), cannon.GetMovingDirection(), cannon.GetAngle());
}

CannonState* CannonUp60State::Update(Cannon& cannon)
{
	float shootingAngle = cannon.CalculateShootingAngle();

	if (shootingAngle > Constants::Enemies::Cannon::AIM_ANGLE_THRESHOLD_45_DEGREES && cannon.IsTargetInRange())
		return new CannonUp30State();

	return nullptr;
}
