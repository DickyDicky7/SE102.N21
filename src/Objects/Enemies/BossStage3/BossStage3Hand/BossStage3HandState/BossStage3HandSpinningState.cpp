#include "BossStage3Hand.h"

BossStage3HandSpinningState::BossStage3HandSpinningState(BossStage3Hand& bossStage3Hand) : BossStage3HandState()
{
	this->_speedAngle = 0.0f;
	this->_numRound = 0.0f;
	this->_totalFrame = 0;
	this->_delayChangeState = 0;
	this->_frameAttack = 0;
	this->_joints = nullptr;
}

BossStage3HandSpinningState::~BossStage3HandSpinningState()
{
}


void BossStage3HandSpinningState::Exit(BossStage3Hand& bossStage3Hand)
{
}

void BossStage3HandSpinningState::Enter(BossStage3Hand& bossStage3Hand)
{
	this->_frameAttack = Constants::Enemies::BossStage3::Hand::ATTACK_FRAME_DURATION;

	this->_speedAngle = Constants::Enemies::BossStage3::Hand::SPIN_SPEED_ANGLE;
	this->_totalFrame = static_cast<int>(Constants::Enemies::BossStage3::Hand::SPIN_TOTAL_FRAME_DIVIDEND / this->_speedAngle);
	this->_delayChangeState = Constants::Enemies::BossStage3::Hand::SPIN_DELAY_CHANGE_STATE_FRAMES;

	if (bossStage3Hand.GetMovingDirection() == DIRECTION::LEFT)
	{
		this->MoveAround(bossStage3Hand.GetJoint(1), bossStage3Hand.GetJoint(0), 0, this->_speedAngle,
			static_cast<float>(this->_totalFrame), BossStage3Joint::MoveAroundDirection::Positive);
	}
	else
	{
		this->MoveAround(bossStage3Hand.GetJoint(1), bossStage3Hand.GetJoint(0), 0, this->_speedAngle,
			static_cast<float>(this->_totalFrame), BossStage3Joint::MoveAroundDirection::Negative);
	}

	this->_joints = bossStage3Hand.GetJoints();
}

void BossStage3HandSpinningState::MoveAround(BossStage3Joint* joint, BossStage3Joint* joint0, float deltaRadius,
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




BossStage3HandState* BossStage3HandSpinningState::Update(BossStage3Hand& bossStage3Hand)
{
	if (this->_frameAttack >= 0)
	{
		this->_frameAttack--;
	}
	else
	{
		if (bossStage3Hand.GetTarget())
		{
			float angle = 0;

			// Aim straight at Bill to fire
			float xBill = bossStage3Hand.GetTarget()->GetPosition().x;
			float yBill = bossStage3Hand.GetTarget()->GetPosition().y;
			D3DXVECTOR3 j = bossStage3Hand.GetJoint(4)->GetPosition();
			angle = this->GetAngle(D3DXVECTOR2(xBill, yBill), D3DXVECTOR2(j.x, j.y));

			bossStage3Hand.Fire(bossStage3Hand.GetJoint(4)->GetX(), bossStage3Hand.GetJoint(4)->GetY(), angle, Constants::Enemies::BossStage3::Hand::ATTACK_FIRE_ANGLE_OFFSET);
		}
		this->_frameAttack = Constants::Enemies::BossStage3::Hand::ATTACK_FRAME_DURATION;
	}

	if (!bossStage3Hand.GetJoint(1)->IsMoveAround())
	{
		bossStage3Hand.GetJoint(4)->SetMoveAroundSpeed(bossStage3Hand.GetJoint(4)->GetMoveAroundSpeed() * Constants::Enemies::BossStage3::Hand::JOINT_4_SPEED_DECAY);
	}

	if (!bossStage3Hand.GetJoint(4)->IsMoveAround())
	{
		if (this->_delayChangeState >= 0)
		{
			this->_delayChangeState--;
		}
		else
		{
			return new BossStage3HandAttackState(bossStage3Hand);
		}
	}

	for (size_t i = 0; i < Constants::Enemies::BossStage3::Hand::TOTAL_JOINTS_COUNT; i++)
	{
		bossStage3Hand.GetJoint(i)->Update();
	}
	return nullptr;
}

BossStage3HandState* BossStage3HandSpinningState::HandleInput(BossStage3Hand& bossStage3Hand, Input& input)
{
	return nullptr;
}
