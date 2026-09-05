#include <cmath>
#include "BossStage3Hand.h"

BossStage3HandDirectPlayerState::BossStage3HandDirectPlayerState(BossStage3Hand& bossStage3Hand) : BossStage3HandState()
{
	// Moved out of the destructor, where the stores were dead and left the
	// object uninitialised between construction and Enter().
	this->_joints = nullptr;
	this->_speed = 0.0f;
	this->_isFirstTime = false;
	this->_frameDelayChangeState = 0;
	this->_timeAttack = 0;
}

BossStage3HandDirectPlayerState::~BossStage3HandDirectPlayerState()
{
}


void BossStage3HandDirectPlayerState::Exit(BossStage3Hand& bossStage3Hand)
{
}

void BossStage3HandDirectPlayerState::Enter(BossStage3Hand& bossStage3Hand)
{
	this->_isFirstTime = true;

	this->_timeAttack = Constants::Enemies::BossStage3::Hand::DIRECT_PLAYER_ATTACK_TIME_FRAMES;

	this->_joints = bossStage3Hand.GetJoints();
	this->_speed = Constants::Enemies::BossStage3::Hand::ATTACK_SPEED;
	this->_frameDelayChangeState = Constants::Enemies::BossStage3::Hand::DIRECT_PLAYER_DELAY_FRAMES;

	for (size_t i = 2; i < Constants::Enemies::BossStage3::Hand::TOTAL_JOINTS_COUNT; i++)
	{
		this->_joints[i]->MoveFollow(this->_joints[i - 1], false);
	}

	for (size_t i = 1; i < Constants::Enemies::BossStage3::Hand::TOTAL_JOINTS_COUNT; i++)
	{
		this->_joints[i]->StopMoveFollow();
		this->MoveAroundDirect(bossStage3Hand, this->_joints[0], this->_joints[i], this->_speed, Constants::Enemies::BossStage3::Hand::DIRECT_MOVE_ANGLE_FACTOR * static_cast<float>(i));
	}
}



BossStage3HandState* BossStage3HandDirectPlayerState::Update(BossStage3Hand& bossStage3Hand)
{
	if (this->_timeAttack > 0)
	{
		this->_timeAttack--;

		if (this->_timeAttack == 0)
		{
			D3DXVECTOR3 a = this->GetNearestPlayer(bossStage3Hand);
			D3DXVECTOR3 b = bossStage3Hand.GetJoint(4)->GetPosition();

			float angle = this->GetAngle(D3DXVECTOR2(a.x, a.y), D3DXVECTOR2(b.x, b.y));

			bossStage3Hand.Fire(bossStage3Hand.GetJoint(4)->GetX(), bossStage3Hand.GetJoint(4)->GetY(), angle, Constants::Enemies::BossStage3::Hand::ATTACK_FIRE_ANGLE_OFFSET);
		}
	}

	this->_frameDelayChangeState--;

	if (this->_frameDelayChangeState <= 0)
	{
		return new BossStage3HandWaveState(bossStage3Hand);
	}

	if (!this->_joints[1]->IsMoveAround())
	{
		this->_isFirstTime = false;
	}

	for (size_t i = 1; i < Constants::Enemies::BossStage3::Hand::TOTAL_JOINTS_COUNT; i++)
	{
		if (!this->_joints[i]->IsMoveAround())
		{
			if (std::abs(this->GetAngleBetweenPlayerAndJoint(this->_joints[i], bossStage3Hand)) >= Constants::Enemies::BossStage3::Hand::DIRECT_PLAYER_ANGLE_TOLERANCE_DEGREES)
			{
				this->MoveAroundDirect(bossStage3Hand, this->_joints[0], this->_joints[i], this->_speed, Constants::Enemies::BossStage3::Hand::DIRECT_MOVE_ANGLE_FACTOR * static_cast<float>(i));
			}
		}
	}

	for (size_t i = 2; i < Constants::Enemies::BossStage3::Hand::TOTAL_JOINTS_COUNT; i++)
	{
		this->_joints[i]->AlignDistance(this->_joints[i - 1]);
	}

	for (size_t i = 0; i < Constants::Enemies::BossStage3::Hand::TOTAL_JOINTS_COUNT; i++)
	{
		bossStage3Hand.GetJoint(i)->Update();
	}
	return nullptr;
}

BossStage3HandState* BossStage3HandDirectPlayerState::HandleInput(BossStage3Hand& bossStage3Hand, Input& input)
{
	return nullptr;
}


float BossStage3HandDirectPlayerState::GetAngle2Vector(D3DXVECTOR3 vec1, D3DXVECTOR3 vec2)
{
	// atan2 reproduces the old acos(x) * sign(y) exactly for these vectors, but
	// sign(y) was spelled abs(y)/y, which is 0/0 = NaN for a horizontal or a
	// zero-length vector.  atan2 needs no normalise, so both are gone.
	//
	// The result is still a raw difference, so it is not wrapped to (-pi, pi] -
	// that is the pre-existing behaviour the callers are tuned against, and it
	// is deliberately left alone here.
	float angle1 = std::atan2(vec1.y, vec1.x);
	float angle2 = std::atan2(vec2.y, vec2.x);

	float result = angle2 - angle1;

	return result;
}

float BossStage3HandDirectPlayerState::GetAngleBetweenPlayerAndJoint(BossStage3Joint* joint, BossStage3Hand& bossStage3Hand)
{
	D3DXVECTOR3 playerPos;
	playerPos = this->GetNearestPlayer(bossStage3Hand);

	float angle = D3DXToDegree(this->GetAngle2Vector(joint->GetPosition() - this->_joints[0]->GetPosition(), playerPos - this->_joints[0]->GetPosition()));

	return angle;
}

void BossStage3HandDirectPlayerState::MoveAroundDirect(BossStage3Hand& bossStage3Hand, BossStage3Joint* joint0, BossStage3Joint* joint, float speed, float radius)
{
	D3DXVECTOR3 playerPos;
	playerPos = this->GetNearestPlayer(bossStage3Hand);

	float angle = D3DXToDegree(this->GetAngle2Vector(joint->GetPosition() - this->_joints[0]->GetPosition(), playerPos - this->_joints[0]->GetPosition()));

	int frameMove = static_cast<int>(std::abs(angle / speed));

	if (this->_isFirstTime)
	{
		// On the first pass the left hand swings left and the right hand swings right;
		// after that each moves by the angular offset instead.
		// Angular offset.
		if (bossStage3Hand.GetMovingDirection() == DIRECTION::LEFT)
		{
			if (angle > 0)
				frameMove = static_cast<int>(std::abs((Constants::Physics::FULL_CIRCLE_DEGREES - angle) / speed));

			joint->MoveAround(joint0->GetPosition().x, joint0->GetPosition().y, radius, frameMove, speed, BossStage3Joint::MoveAroundDirection::Negative);
		}
		else
		{
			joint->MoveAround(joint0->GetPosition().x, joint0->GetPosition().y, radius, frameMove, speed, BossStage3Joint::MoveAroundDirection::Positive);
		}
	}
	else
	{
		if (angle < 0)
		{
			joint->MoveAround(joint0->GetPosition().x, joint0->GetPosition().y, radius, frameMove, speed, BossStage3Joint::MoveAroundDirection::Negative);
		}
		else
		{
			joint->MoveAround(joint0->GetPosition().x, joint0->GetPosition().y, radius, frameMove, speed, BossStage3Joint::MoveAroundDirection::Positive);
		}
	}
}

D3DXVECTOR3 BossStage3HandDirectPlayerState::GetNearestPlayer(BossStage3Hand& bossStage3Hand)
{
	if (!bossStage3Hand.GetTarget()) return D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	float xBill = bossStage3Hand.GetTarget()->GetPosition().x;
	float yBill = bossStage3Hand.GetTarget()->GetPosition().y;
	return  D3DXVECTOR3(xBill, yBill, 0);
}
