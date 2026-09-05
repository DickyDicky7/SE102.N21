#include <cmath>
#include "BossStage3Joint.h"
#include "BossStage3Hand.h"

BossStage3Joint::BossStage3Joint(BOSS_STAGE_3_HAND_ANIMATION_ID type, D3DXVECTOR3 positionInit, DIRECTION direction, BossStage3Hand* parent) : Entity(), HasTextures(), HasSprites()
{
	this->_w = Constants::Enemies::BossStage3::Hand::JOINT_WIDTH;
	this->_h = Constants::Enemies::BossStage3::Hand::JOINT_HEIGHT;

	this->_vx = 0;
	this->_vy = 0;
	this->_ax = Constants::Physics::DEFAULT_INITIAL_ACCELERATION_X;
	this->_ay = Constants::Physics::DEFAULT_INITIAL_ACCELERATION_Y;
	this->_position = positionInit;
	this->SetDebugName(L"BossStage3Joint\n");

	this->_movingDirection = DIRECTION::LEFT;

	this->_type = type;

	this->_moveAroundDirection = MoveAroundDirection::Unknown;
	this->_distanceMoved = 0.0f;
	this->_angleMoved = 0.0f;
	this->_moveAroundAccelerator = 0;

	this->_isMoveAround = false;
	this->_isMoveTo = false;
	this->_isMoveBy = false;
	this->_isMoveFollow = false;
	this->_allowMoveReverse = false;
	this->_isMoveAroundAccelerator = false;
	this->_isJointFollowStopped = true;
	this->_isMoveFollowFirstTime = false;
	this->_isMoveFollowWithAccelerator = true;

	this->_jointFollow = nullptr;

	// The isMove* flags above gate every read of these, and moveAround/moveTo/
	// moveBy set them alongside the flag - but leaving them indeterminate makes
	// the object's state undefined the moment anything copies or inspects it.
	this->_moveAroundX = 0.0f;
	this->_moveAroundY = 0.0f;
	this->_moveAroundRadius = 0.0f;
	this->_moveAroundSpeed = 0.0f;
	this->_moveAroundFrame = 0;
	this->_moveAroundCurrFrame = 0;
	this->_moveAroundDelay = 0;
	this->_moveToX = 0.0f;
	this->_moveToY = 0.0f;
	this->_moveToFrame = 0;
	this->_moveToCurrFrame = 0;
	this->_moveToDelay = 0;
	this->_moveByFrame = 0;
	this->_moveByCurrFrame = 0;
	this->_moveByDelay = 0;

	this->_parent = parent;
	this->_hitCounts = (std::numeric_limits<int>::max)();
	this->_enemyType = ENEMY_TYPE::BOSS;
}

BossStage3Joint::~BossStage3Joint()
{
}

void BossStage3Joint::Update()
{
	float oldx = this->GetX();
	float oldy = this->GetY();

	if (this->_isMoveAround)
	{
		this->UpdateMoveAround();
	}

	if (this->_isMoveTo)
	{
		this->UpdateMoveTo(oldx, oldy);
	}

	if (this->_isMoveBy)
	{
		this->UpdateMoveBy(oldx, oldy);
	}

	if (this->_isMoveFollow)
	{
		this->UpdateMoveFollow();
	}

	this->_currMoveDirection = D3DXVECTOR2(this->GetX() - oldx, this->GetY() - oldy);
}

void BossStage3Joint::Render()
{
	this->SetAnimation(this->_type, this->GetPosition(), this->GetMovingDirection(), this->GetAngle());
	this->_w = this->GetCurrentFrameW();
	this->_h = this->GetCurrentFrameH();
}

void BossStage3Joint::HandleInput(Input& input)
{
}

void BossStage3Joint::LoadSprites()
{
	// sprites had loaded
}

void BossStage3Joint::LoadTextures()
{
	// textures had loaded
}

void BossStage3Joint::LoadAnimations()
{
	// animation had loaded in hand parent
}


void BossStage3Joint::UpdateMoveAround()
{
	if (this->_moveAroundDelay > 0)
	{
		this->_moveAroundDelay--;
	}
	else
	{
		this->_moveAroundCurrFrame++;

		// Rotate around a point.
		//
		// atan2 gives the same signed angle the old acos(x) * sign(y) did, but
		// without the singularity: sign(y) was written as abs(y)/y, which is
		// 0/0 = NaN whenever the joint sits exactly level with the centre - and
		// D3DXVec2Normalize hands back (0,0) for a zero-length vector, so two
		// coincident joints landed there too.  A NaN here is permanent: it goes
		// straight into SetX/SetY, and next frame the length test (len > 0.0f)
		// is false for NaN, which normalises back to (0,0) and NaNs again.
		// atan2 is also scale-invariant, so the normalise is no longer needed.
		D3DXVECTOR2 pos(this->GetX() - this->_moveAroundX, this->GetY() - this->_moveAroundY);

		float angle = std::atan2(pos.y, pos.x);

		angle += D3DXToRadian(this->_moveAroundDirection == MoveAroundDirection::Positive ? this->_moveAroundSpeed : -this->_moveAroundSpeed);

		float posx = std::cos(angle);
		float posy = std::sin(angle);

		this->SetX(this->_moveAroundX + posx * this->_moveAroundRadius);
		this->SetY(this->_moveAroundY + posy * this->_moveAroundRadius);

		this->_distanceMoved = std::abs((Constants::Physics::TURN_RADIANS * this->_moveAroundRadius * this->_moveAroundSpeed) / Constants::Physics::FULL_CIRCLE_DEGREES);
		this->_angleMoved += this->_moveAroundSpeed;

		if (this->_moveAroundFrame != -1)
		{
			if (this->_moveAroundCurrFrame >= this->_moveAroundFrame)
			{
				this->_isMoveAround = false;
			}
		}

		if (this->_moveAroundSpeed <= 0)
		{
			this->_isMoveAround = false;
		}

		this->SetMoveAroundSpeed(this->_moveAroundSpeed - this->_moveAroundAccelerator);
	}
}


void BossStage3Joint::UpdateMoveBy(float oldx, float oldy)
{
	if (this->_moveByDelay >= 0)
	{
		this->_moveByDelay--;
	}
	else
	{
		this->_moveByCurrFrame++;

		this->SetX(this->GetX() + this->GetVX());
		this->SetY(this->GetY() + this->GetVY());

		D3DXVECTOR2 dist = D3DXVECTOR2(this->GetX() - oldx, this->GetY() - oldy);
		this->_distanceMoved += D3DXVec2Length(&dist);

		if (this->_moveByCurrFrame >= this->_moveByFrame)
		{
			this->SetVX(0);
			this->SetVY(0);
			this->_isMoveBy = false;
		}
	}
}

void BossStage3Joint::UpdateMoveTo(float oldx, float oldy)
{
	if (this->_moveToDelay > 0)
	{
		this->_moveToDelay--;
	}
	else
	{
		this->_moveToCurrFrame++;

		this->SetX(this->GetX() + this->GetVX());
		this->SetY(this->GetY() + this->GetVY());

		D3DXVECTOR2 dist = D3DXVECTOR2(this->GetX() - oldx, this->GetY() - oldy);
		this->_distanceMoved += D3DXVec2Length(&dist);

		if (this->_moveToCurrFrame >= this->_moveToFrame)
		{
			this->SetX(this->_moveToX);
			this->SetY(this->_moveToY);

			this->SetVX(0);
			this->SetVY(0);

			this->_isMoveTo = false;

		}
	}
}

void BossStage3Joint::UpdateMoveFollow()
{
	// Change movement direction
	if (this->_moveAroundDirection != this->_jointFollow->_moveAroundDirection)
	{
		if (!this->_allowMoveReverse && this->_jointFollow->_isMoveAround)
		{
			if (this->_isMoveFollowFirstTime)
			{
				if (this->_isMoveFollowWithAccelerator)
				{
					this->MoveAround(this->_jointFollow->GetX(), this->_jointFollow->GetY(), Constants::Enemies::BossStage3::Hand::JOINT_MOVE_AROUND_RADIUS, Constants::Enemies::BossStage3::Hand::JOINT_FOLLOW_FRAMES_UNBOUNDED, this->_jointFollow->GetAngleSpeed() * Constants::Enemies::BossStage3::Hand::JOINT_FOLLOW_SPEED_RATIO_FIRST, this->_moveAroundDirection, this->_jointFollow->GetAngleSpeed() / Constants::Enemies::BossStage3::Hand::JOINT_FOLLOW_ACCELERATOR_DIVISOR);
					this->_isMoveFollowFirstTime = false;
				}
			}
			else
			{
				if (this->_isMoveFollowWithAccelerator)
				{
					this->MoveAround(this->_jointFollow->GetX(), this->_jointFollow->GetY(), Constants::Enemies::BossStage3::Hand::JOINT_MOVE_AROUND_RADIUS, Constants::Enemies::BossStage3::Hand::JOINT_FOLLOW_FRAMES_UNBOUNDED, this->_jointFollow->GetAngleSpeed() * Constants::Enemies::BossStage3::Hand::JOINT_FOLLOW_SPEED_RATIO_REPEAT, this->_moveAroundDirection, this->_jointFollow->GetAngleSpeed() / Constants::Enemies::BossStage3::Hand::JOINT_FOLLOW_ACCELERATOR_DIVISOR);
				}
			}

			this->_allowMoveReverse = true;
		}
	}


	if (this->_allowMoveReverse && this->_jointFollow->_isMoveAround)
	{
		if (this->_moveAroundSpeed <= 0)
		{
			this->ReverseMoveAroundDirection();
			this->SetMoveAroundSpeed(this->_jointFollow->GetAngleSpeed());
			this->_allowMoveReverse = false;
		}
	}

	if (!this->_allowMoveReverse)
	{
		if (this->_jointFollow->_isMoveAround)
		{
			if (this->_isMoveFollowWithAccelerator)
			{
				this->MoveAround(this->_jointFollow->GetX(), this->_jointFollow->GetY(), Constants::Enemies::BossStage3::Hand::JOINT_MOVE_AROUND_RADIUS, static_cast<int>(this->_jointFollow->GetAngleSpeed() * Constants::Enemies::BossStage3::Hand::JOINT_FOLLOW_FRAME_SPEED_FACTOR), this->_moveAroundSpeed, this->_moveAroundDirection);
			}
			else
			{
				this->MoveAround(this->_jointFollow->GetX(), this->_jointFollow->GetY(), Constants::Enemies::BossStage3::Hand::JOINT_MOVE_AROUND_RADIUS, Constants::Enemies::BossStage3::Hand::JOINT_MOVE_AROUND_FRAMES, this->_moveAroundSpeed * Constants::Enemies::BossStage3::Hand::JOINT_MOVE_AROUND_SPEED_FACTOR, this->_moveAroundDirection);
			}
		}
	}

	this->SetVX(this->_jointFollow->GetCurrentMoveDirection().x);
	this->SetVY(this->_jointFollow->GetCurrentMoveDirection().y);

	this->SetX(this->GetX() + this->GetVX());
	this->SetY(this->GetY() + this->GetVY());

	this->AlignDistance(this->_jointFollow);
}


void BossStage3Joint::MoveBy(float x, float y, int frames, int delayFrames)
{
	this->_distanceMoved = 0.0f;

	this->SetVX(x);
	this->SetVY(y);

	this->_moveByFrame = frames;
	this->_moveByDelay = delayFrames;
	this->_moveByCurrFrame = 0;

	this->_isMoveBy = true;
}

void BossStage3Joint::MoveAround(float x, float y, float radius, int frames, float speed, MoveAroundDirection direction, float accelerator, int delayFrames)
{
	this->_distanceMoved = 0.0f;
	this->_angleMoved = 0.0f;
	this->_moveAroundX = x;
	this->_moveAroundY = y;
	this->_moveAroundFrame = frames;
	this->_moveAroundCurrFrame = 0;
	this->_moveAroundDelay = delayFrames;
	this->_moveAroundAccelerator = accelerator;
	this->_isMoveAround = true;

	if (accelerator != 0)
	{
		this->_isMoveAroundAccelerator = true;
	}
	else
	{
		this->_isMoveAroundAccelerator = false;
	}

	this->_moveAroundDirection = direction;
	this->SetMoveAroundRadius(radius);
	this->SetMoveAroundSpeed(speed);
}

void BossStage3Joint::MoveFollow(BossStage3Joint* joint, bool isMoveFollowWithAccelerator)
{
	this->_jointFollow = joint;
	this->_isMoveFollow = true;
	this->_isMoveFollowFirstTime = true;
	this->_isMoveFollowWithAccelerator = isMoveFollowWithAccelerator;

	if (joint->_isMoveAround)
	{
		this->_isJointFollowStopped = false;
	}
	else
	{
		this->_isJointFollowStopped = true;
	}

	this->SetVX(joint->GetCurrentMoveDirection().x);
	this->SetVY(joint->GetCurrentMoveDirection().y);
	this->_moveAroundDirection = joint->_moveAroundDirection == MoveAroundDirection::Positive ? MoveAroundDirection::Negative : MoveAroundDirection::Positive;
}

void BossStage3Joint::AlignDistance(BossStage3Joint* joint2)
{
	D3DXVECTOR3 vect = (this->GetPosition() - joint2->GetPosition());
	D3DXVECTOR2 vec = D3DXVECTOR2(vect.x, vect.y);
	float distance = Constants::Enemies::BossStage3::Hand::JOINT_MOVE_AROUND_RADIUS;

	D3DXVec2Normalize(&vec, &vec);

	// The angle / posx / posy trio that used to sit here was never read - the
	// placement below works off vec directly - so its abs(y)/y NaN was invisible.
	// Dropping it also clears two "initialized but not referenced" warnings.

	float joint1X = joint2->GetPosition().x + vec.x * distance;
	float joint1Y = joint2->GetPosition().y + vec.y * distance;

	this->SetX(joint1X);
	this->SetY(joint1Y);
}

void BossStage3Joint::StopMoveAround()
{
	this->_isMoveAround = false;
	this->_moveAroundSpeed = 0;
	this->_moveAroundAccelerator = 0;
	this->_moveAroundCurrFrame = 0;
	this->_angleMoved = 0;
}

void BossStage3Joint::SetMoveAroundSpeed(float speed)
{
	if (speed <= 0)
		this->_moveAroundSpeed = 0;
	else
		this->_moveAroundSpeed = speed;
}

void BossStage3Joint::SetMoveAroundRadius(float radius)
{
	this->_moveAroundRadius = radius;
}

D3DXVECTOR2 BossStage3Joint::GetCurrentMoveDirection()
{
	return this->_currMoveDirection;
}

float BossStage3Joint::GetAngleSpeed()
{
	return this->_moveAroundSpeed;
}

void BossStage3Joint::ReverseMoveAroundDirection()
{
	if (this->_moveAroundDirection == MoveAroundDirection::Positive)
	{
		this->_moveAroundDirection = MoveAroundDirection::Negative;
	}
	else
	{
		this->_moveAroundDirection = MoveAroundDirection::Positive;
	}

	this->_angleMoved = 0.0f;
}

void BossStage3Joint::SetMoveAroundDelay(int frame)
{
	this->_moveAroundDelay = frame;
}

void BossStage3Joint::StopMoveFollow()
{
	this->_isMoveFollow = false;
	this->_jointFollow = nullptr;
}

int BossStage3Joint::GetMoveAroundCurrFrame()
{
	return this->_moveAroundCurrFrame;
}

bool BossStage3Joint::TakeBulletHit()
{
	if (this->_parent && this->_parent->RegisterHit())
	{
		this->_parent->SetDead(true);
	}
	return true;
}
