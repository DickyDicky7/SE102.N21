#include <numbers>
#include "RifleManStanding.h"

RifleManStandingNormalState::RifleManStandingNormalState()
{

}

RifleManStandingNormalState::~RifleManStandingNormalState()
{

}

void RifleManStandingNormalState::Exit(RifleManStanding& rifleManStanding)
{

}

void RifleManStandingNormalState::Enter(RifleManStanding& rifleManStanding)
{
	// UpdateShooting() advances the burst counters, so the target check has to
	// come first: otherwise the round is consumed and then silently dropped.
	const Bill* target = rifleManStanding.GetEnemyTarget();
	if (!target) return;

	if (this->UpdateShooting(rifleManStanding))
	{
		D3DXVECTOR3 position = rifleManStanding.GetPosition();

		float w = rifleManStanding.GetW();
		float h = rifleManStanding.GetH();

		DIRECTION movingDirection = rifleManStanding.GetMovingDirection();

		float shootingAngle = D3DXToRadian(std::abs(rifleManStanding.CalculateShootingAngle()));

		float dy = -((rifleManStanding.GetPosition().y) - (target->GetPosition().y));
		float signY = (std::abs(dy) > Constants::Physics::DIRECTION_EPSILON) ? (dy / std::abs(dy)) : 0.0f;
		float vy = signY;

		float vx = movingDirection == DIRECTION::LEFT ? -1.0f : 1.0f;
		float tanValue = std::tan(std::numbers::pi_v<float> / 2 - shootingAngle);

		vy = vy * tanValue;

		if (tanValue > 1.0f && std::abs(vy) > Constants::Physics::DIRECTION_EPSILON)
		{
			vx = signY * vx / vy;
			vy = signY;
		}

		if (movingDirection == DIRECTION::LEFT)
		{
			rifleManStanding.CustomFire(position.x - w * 0.5f, position.y + h * Constants::Enemies::RifleMan::NORMAL_GUN_OFFSET_RATIO_Y, 0.0f, vx, vy, 0.0f, 0.0f, movingDirection);
		}
		else
		{
			rifleManStanding.CustomFire(position.x + w * 0.5f, position.y + h * Constants::Enemies::RifleMan::NORMAL_GUN_OFFSET_RATIO_Y, 0.0f, vx, vy, 0.0f, 0.0f, movingDirection);
		}
	}
}

void RifleManStandingNormalState::Render(RifleManStanding& rifleManStanding)
{
	rifleManStanding.SetAnimation(RIFLE_MAN_ANIMATION_ID::SHOOT_NORMAL, rifleManStanding.GetPosition(), rifleManStanding.GetMovingDirection(), rifleManStanding.GetAngle());
}

RifleManStandingState* RifleManStandingNormalState::Update(RifleManStanding& rifleManStanding)
{
	return nullptr;
}
