#include <cmath>
#include "BossStage3Hand.h"

BossStage3HandWaveState::BossStage3HandWaveState(BossStage3Hand& bossStage3Hand) : BossStage3HandState()
{
	// Moved out of the destructor, where the stores were dead and left the
	// object uninitialised between construction and Enter().
	this->_limitFrame = 0.0f;
	this->_speed = 0.0f;
	this->_currFrame = 0;
	this->_changeStateFrame = 0;
	this->_moveAroundDelay = 0;
}

BossStage3HandWaveState::~BossStage3HandWaveState()
{
}


void BossStage3HandWaveState::Exit(BossStage3Hand& bossStage3Hand)
{
}

void BossStage3HandWaveState::Enter(BossStage3Hand& bossStage3Hand)
{
	this->_moveAroundDelay = 0;

	this->_speed = Constants::Enemies::BossStage3::Hand::WAVE_SPEED;
	this->_currFrame = 0;

	if (bossStage3Hand.GetMovingDirection() == DIRECTION::LEFT)
	{
		this->MoveAround(bossStage3Hand.GetJoint(1), bossStage3Hand.GetJoint(0), 0, this->_speed,
			-1, BossStage3Joint::MoveAroundDirection::Negative);
	}
	else
	{
		this->MoveAround(bossStage3Hand.GetJoint(1), bossStage3Hand.GetJoint(0), 0, this->_speed,
			-1, BossStage3Joint::MoveAroundDirection::Positive);
	}

	bossStage3Hand.GetJoint(2)->MoveFollow(bossStage3Hand.GetJoint(1));
	bossStage3Hand.GetJoint(3)->MoveFollow(bossStage3Hand.GetJoint(2));
	bossStage3Hand.GetJoint(4)->MoveFollow(bossStage3Hand.GetJoint(3));
}



BossStage3HandState* BossStage3HandWaveState::Update(BossStage3Hand& bossStage3Hand)
{
	D3DXVECTOR3 a = bossStage3Hand.GetJoint(1)->GetPosition();
	D3DXVECTOR3 b = bossStage3Hand.GetJoint(0)->GetPosition();

	float angle = this->GetAngle(D3DXVECTOR2(a.x, a.y), D3DXVECTOR2(b.x,b.y));

	switch (bossStage3Hand.GetJoint(1)->GetMoveAroundDirection())
	{
	case BossStage3Joint::MoveAroundDirection::Positive:
		// Change movement direction

		if (bossStage3Hand.GetMovingDirection() == DIRECTION::LEFT)
		{
			// Reverse the left hand's movement direction
			// Positive = the left hand rotates upward
			if (angle < 0 && angle > D3DXToRadian(Constants::Enemies::BossStage3::Hand::WAVE_REVERSE_ANGLE_LEFT_POS_DEGREES))
			{
				this->_currFrame++;
				bossStage3Hand.GetJoint(1)->SetMoveAroundSpeed(this->_speed);
				bossStage3Hand.GetJoint(1)->ReverseMoveAroundDirection();
				bossStage3Hand.GetJoint(1)->SetMoveAroundDelay(this->_moveAroundDelay);
			}
		}
		else
		{
			// Right hand
			// Positive = the right hand rotates downward
			if (angle > 0 && angle > D3DXToRadian(Constants::Enemies::BossStage3::Hand::WAVE_REVERSE_ANGLE_RIGHT_POS_DEGREES))
			{
				this->_currFrame++;
				bossStage3Hand.GetJoint(1)->SetMoveAroundSpeed(this->_speed);
				bossStage3Hand.GetJoint(1)->ReverseMoveAroundDirection();
				bossStage3Hand.GetJoint(1)->SetMoveAroundDelay(this->_moveAroundDelay);
			}
		}

		break;

	case BossStage3Joint::MoveAroundDirection::Negative:
		// Change movement direction
		// Mirrors the Positive case above
		if (bossStage3Hand.GetMovingDirection() == DIRECTION::LEFT)
		{
			if (angle > 0 && angle < D3DXToRadian(Constants::Enemies::BossStage3::Hand::WAVE_REVERSE_ANGLE_LEFT_NEG_DEGREES))
			{
				this->_currFrame++;
				bossStage3Hand.GetJoint(1)->SetMoveAroundSpeed(this->_speed);
				bossStage3Hand.GetJoint(1)->ReverseMoveAroundDirection();
				bossStage3Hand.GetJoint(1)->SetMoveAroundDelay(this->_moveAroundDelay);
			}
		}
		else
		{
			if (angle < 0 && angle < D3DXToRadian(Constants::Enemies::BossStage3::Hand::WAVE_REVERSE_ANGLE_RIGHT_NEG_DEGREES))
			{
				this->_currFrame++;
				bossStage3Hand.GetJoint(1)->SetMoveAroundSpeed(this->_speed);
				bossStage3Hand.GetJoint(1)->ReverseMoveAroundDirection();
				bossStage3Hand.GetJoint(1)->SetMoveAroundDelay(this->_moveAroundDelay);
			}
		}
		break;
	}

	if (this->_currFrame >= Constants::Enemies::BossStage3::Hand::WAVE_MAX_CYCLES)
	{
		return new BossStage3HandSpinningState(bossStage3Hand);
	}

	for (size_t i = 0; i < Constants::Enemies::BossStage3::Hand::TOTAL_JOINTS_COUNT; i++)
	{
		bossStage3Hand.GetJoint(i)->Update();
	}
	return nullptr;
}

BossStage3HandState* BossStage3HandWaveState::HandleInput(BossStage3Hand& bossStage3Hand, Input& input)
{
	return nullptr;
}


void BossStage3HandWaveState::MoveAround(BossStage3Joint* joint, BossStage3Joint* joint0, float deltaRadius,
	float speed, float frame, BossStage3Joint::MoveAroundDirection dir, float accelerator)
{
	float x = joint0->GetX() + joint0->GetW() / 2;
	float y = joint0->GetY() + joint0->GetH() / 2;

	float x2 = joint->GetX() + joint->GetW() / 2;
	float y2 = joint->GetY() + joint->GetH() / 2;
	D3DXVECTOR2 dist(x - x2, y - y2);

	// The outer joints stop before the inner ones, and the hand stops first.
	// Pull the joint-to-hand distances back in: rotating stretches them out.
	// The wave effect needs a different speed per joint, innermost fastest.
	joint->MoveAround(x, y, D3DXVec2Length(&dist) - deltaRadius, static_cast<int>(frame), speed, dir, accelerator);
}

float BossStage3HandWaveState::GetAngle(D3DXVECTOR2 pos1, D3DXVECTOR2 pos2)
{
	D3DXVECTOR2 vec(pos1 - pos2);

	// Same signed angle as the old acos(x) * sign(y), minus the 0/0 = NaN that
	// sign(y) produced whenever the two points were exactly level (or equal).
	// A NaN here silently disabled the wave reversal, since every comparison
	// against it is false.  atan2 is scale-invariant, so no normalise needed.
	float angle = std::atan2(vec.y, vec.x);

	return angle;
}
