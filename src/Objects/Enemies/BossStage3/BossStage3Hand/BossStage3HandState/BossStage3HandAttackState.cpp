#include "BossStage3Hand.h"

BossStage3HandAttackState::BossStage3HandAttackState(BossStage3Hand& bossStage3Hand) : BossStage3HandState()
{
	// Moved out of the destructor, where the stores were dead and left the
	// object uninitialised between construction and Enter().
	this->_delayChangeState = 0;
	this->_frameAttack = 0;
	this->_joints = nullptr;
}

BossStage3HandAttackState::~BossStage3HandAttackState()
{
}


void BossStage3HandAttackState::Exit(BossStage3Hand& bossStage3Hand)
{
}

void BossStage3HandAttackState::Enter(BossStage3Hand& bossStage3Hand)
{
	float speed = Constants::Enemies::BossStage3::Hand::ATTACK_SPEED;
	float limitFrame = Constants::Enemies::BossStage3::Hand::ATTACK_LIMIT_FRAME_DIVIDEND / speed;
	this->_frameAttack = Constants::Enemies::BossStage3::Hand::ATTACK_FRAME_DURATION;
	this->_delayChangeState = Constants::Enemies::BossStage3::Hand::ATTACK_STATE_DELAY_FRAMES;

	this->_joints = bossStage3Hand.GetJoints();

	if (bossStage3Hand.GetMovingDirection() == DIRECTION::LEFT)
	{
		this->MoveAround(bossStage3Hand.GetJoint(1), bossStage3Hand.GetJoint(0), 0, speed,
			limitFrame, BossStage3Joint::MoveAroundDirection::Negative);
	}
	else
	{
		this->MoveAround(bossStage3Hand.GetJoint(1), bossStage3Hand.GetJoint(0), 0, speed,
			limitFrame, BossStage3Joint::MoveAroundDirection::Positive);
	}

	bossStage3Hand.GetJoint(2)->MoveFollow(bossStage3Hand.GetJoint(1));
	bossStage3Hand.GetJoint(3)->MoveFollow(bossStage3Hand.GetJoint(2));
	bossStage3Hand.GetJoint(4)->MoveFollow(bossStage3Hand.GetJoint(3));
}



BossStage3HandState* BossStage3HandAttackState::Update(BossStage3Hand& bossStage3Hand)
{
	if (this->_frameAttack > 0)
	{
		this->_frameAttack--;

		if (this->_frameAttack == 0 && bossStage3Hand.GetTarget())
		{
			// Fire a bullet from the hand joint
			float xBill = bossStage3Hand.GetTarget()->GetPosition().x;
			float yBill = bossStage3Hand.GetTarget()->GetPosition().y;
			D3DXVECTOR3 j = bossStage3Hand.GetJoint(4)->GetPosition();
			float angle = this->GetAngle(D3DXVECTOR2(xBill, yBill), D3DXVECTOR2(j.x, j.y));

			bossStage3Hand.Fire(bossStage3Hand.GetJoint(4)->GetX(), bossStage3Hand.GetJoint(4)->GetY(),angle, Constants::Enemies::BossStage3::Hand::ATTACK_FIRE_ANGLE_OFFSET);
		}
	}

	if (!bossStage3Hand.GetJoint(1)->IsMoveAround())
	{
		bossStage3Hand.GetJoint(2)->SetMoveAroundSpeed(bossStage3Hand.GetJoint(2)->GetMoveAroundSpeed() * Constants::Enemies::BossStage3::Hand::JOINT_2_SPEED_DECAY);
	}

	if (!bossStage3Hand.GetJoint(2)->IsMoveAround())
	{
		bossStage3Hand.GetJoint(3)->SetMoveAroundSpeed(bossStage3Hand.GetJoint(3)->GetMoveAroundSpeed() * Constants::Enemies::BossStage3::Hand::JOINT_3_SPEED_BOOST);
	}

	if (!bossStage3Hand.GetJoint(4)->IsMoveAround() && !bossStage3Hand.GetJoint(1)->IsMoveAround())
	{
		if (this->_delayChangeState >= 0)
		{
			this->_delayChangeState--;
		}
		else
		{
			return new BossStage3HandDirectPlayerState(bossStage3Hand);
		}
	}

	for (size_t i = 0; i < Constants::Enemies::BossStage3::Hand::TOTAL_JOINTS_COUNT; i++)
	{
		bossStage3Hand.GetJoint(i)->Update();
	}

	return nullptr;
}

BossStage3HandState* BossStage3HandAttackState::HandleInput(BossStage3Hand& bossStage3Hand, Input& input)
{
	return nullptr;
}

void BossStage3HandAttackState::MoveAround(BossStage3Joint* joint, BossStage3Joint* joint0, float deltaRadius,
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
