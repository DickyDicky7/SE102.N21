#include "BossStage3Joint.h"

BossStage3Joint::BossStage3Joint(BOSS_STAGE_3_HAND_ANIMATION_ID type, D3DXVECTOR3 positionInit, DIRECTION direction) : Entity(), HasTextures(), HasSprites()
{
	this->vx = 0;
	this->vy = 0;
	this->ax = 0.1f;
	this->ay = 0.1f;
	this->position = positionInit;
	this->name = L"BossStage3Joint\n";

	this->movingDirection = DIRECTION::LEFT;

	this->type = type;

	moveAroundDirection = MoveAroundDirection::Unknow;
	distanceMoved = 0;
	angleMoved = 0;
	moveAroundAccelemetor = 0;

	isMoveAround = false;
	isMoveTo = false;
	isMoveBy = false;
	isMoveByVelocity = false;
	isMoveFollow = false;
	allowMoveReverse = false;
	isMoveAroundAcclemetor = false;
	isJointFollowStoped = true;
	isMoveFollowFirstTime = false;
	isMoveFollowWithAccelemetor = true;

	jointFollow = nullptr;

    this->hitCounts = 10;
    this->enemyType = ENEMY_TYPE::BOSS;
}

BossStage3Joint::~BossStage3Joint()
{
}

void BossStage3Joint::Update()
{
	float oldx = GetX();
	float oldy = GetY();

	if (isMoveAround)
	{
		updateMoveAround();
	}

	if (isMoveTo)
	{
		updateMoveTo(oldx, oldy);
	}

	if (isMoveBy)
	{
		updateMoveBy(oldx, oldy);
	}

	if (isMoveFollow)
	{
		updateMoveFollow();
	}

	currMoveDirection = D3DXVECTOR2(GetX() - oldx, GetY() - oldy);
}

void BossStage3Joint::Render()
{
	this->w = this->currentFrameW;
	this->h = this->currentFrameH;
	SetAnimation(type, GetPosition(), GetMovingDirection(), GetAngle());
}

void BossStage3Joint::HandleInput(Input& input)
{
	//handleInputState = state->HandleInput(*this, input);
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


void BossStage3Joint::updateMoveAround()
{
	if (moveAroundDelay > 0)
	{
		moveAroundDelay--;
	}
	else
	{
		moveAroundCurrFrame++;

		//xoay xung quanh 1 diem
		D3DXVECTOR2 pos(GetX() - moveAroundX, GetY() - moveAroundY);

		D3DXVec2Normalize(&pos, &pos);

		float angle = acos(pos.x);
		angle *= (abs(pos.y) / pos.y);

		angle += D3DXToRadian(moveAroundDirection == MoveAroundDirection::Positive ? moveAroundSpeed : -moveAroundSpeed);

		float posx = cos(angle);
		float posy = sin(angle);

		SetX(moveAroundX + posx * moveAroundRadius);
		SetY(moveAroundY + posy * moveAroundRadius);

		distanceMoved = abs((2 * 3.1415926f * moveAroundRadius * moveAroundSpeed) / 360);
		angleMoved += moveAroundSpeed;

		if (moveAroundFrame != -1)
		{
			if (moveAroundCurrFrame >= moveAroundFrame)
			{
				isMoveAround = false;
			}
		}

		if (moveAroundSpeed <= 0)
		{
			isMoveAround = false;
		}

		this->setMoveAroundSpeed(moveAroundSpeed - moveAroundAccelemetor);
	}
}


void BossStage3Joint::updateMoveBy(float oldx, float oldy)
{
    if (moveByDelay >= 0)
    {
        moveByDelay--;
    }
    else
    {
        moveByCurrFrame++;

        SetX( GetX() + GetVX());
        SetY( GetY() + GetVY());

        D3DXVECTOR2 dist = D3DXVECTOR2(GetX() - oldx, GetY() - oldy);
        distanceMoved += D3DXVec2Length(&dist);

        if (moveByCurrFrame >= moveByFrame)
        {
            SetVX(0);
            SetVY(0);
            isMoveBy = false;
        }
    }
}

void BossStage3Joint::updateMoveTo(float oldx, float oldy)
{
    if (moveToDelay > 0)
    {
        moveToDelay--;
    }
    else
    {
        moveToCurrFrame++;

        SetX(GetX() + GetVX());
        SetY(GetY() + GetVY());

        D3DXVECTOR2 dist = D3DXVECTOR2(GetX() - oldx, GetY() - oldy);
        distanceMoved += D3DXVec2Length(&dist);

        if (moveToCurrFrame >= moveToFrame)
        {
            SetX(moveToX);
            SetY(moveToY);

            SetVX(0);
            SetVY(0);

            isMoveTo = false;

        }
    }
}

void BossStage3Joint::updateMoveFollow()
{
    //doi huong di chuyen
    if (this->moveAroundDirection != jointFollow->moveAroundDirection)
    {
        if (!allowMoveReverse && jointFollow->isMoveAround)
        {
            if (isMoveFollowFirstTime)
            {
                if (isMoveFollowWithAccelemetor)
                {
                    this->moveAround(jointFollow->GetX(), jointFollow->GetY(), 16, -1, jointFollow->getAngleSpeed() * 0.2f, moveAroundDirection, jointFollow->getAngleSpeed() / 11.0f);
                    isMoveFollowFirstTime = false;
                }
            }
            else
            {
                if (isMoveFollowWithAccelemetor)
                {
                    this->moveAround(jointFollow->GetX(), jointFollow->GetY(), 16, -1, jointFollow->getAngleSpeed() * 0.7f, moveAroundDirection, jointFollow->getAngleSpeed() / 11.0f);
                }
            }

            allowMoveReverse = true;
        }
    }


    if (allowMoveReverse && jointFollow->isMoveAround)
    {
        if (this->moveAroundSpeed <= 0)
        {
            reverseMoveAroundDirection();
            this->setMoveAroundSpeed(jointFollow->getAngleSpeed());
            allowMoveReverse = false;
        }
    }

    if (!allowMoveReverse)
    {
        if (jointFollow->isMoveAround)
        {
            if (isMoveFollowWithAccelemetor)
            {
                this->moveAround(jointFollow->GetX(), jointFollow->GetY(), 16, jointFollow->getAngleSpeed() * 2.5f, moveAroundSpeed, moveAroundDirection);
            }
            else
            {
                this->moveAround(jointFollow->GetX(), jointFollow->GetY(), 16, 5, moveAroundSpeed * 12.5f, moveAroundDirection);
            }
        }
    }

    SetVX(jointFollow->getCurrentMoveDirection().x);
    SetVY(jointFollow->getCurrentMoveDirection().y);

    SetX(GetX() + GetVX());
    SetY(GetY() + GetVY());

    alignDistance(jointFollow);
}


void BossStage3Joint::moveBy(float x, float y, int frames, int delay)
{
    distanceMoved = 0;

    SetVX(x);
    SetVY(y);

    moveByFrame = frames;
    moveByDelay = delay;
    moveByCurrFrame = 0;

    isMoveBy = true;
}

void BossStage3Joint::moveAround(float x, float y, float radius, int frame, float speed, MoveAroundDirection direction, float accelemetor, int delay)
{
    distanceMoved = 0;
    angleMoved = 0;
    moveAroundX = x;
    moveAroundY = y;
    moveAroundFrame = frame;
    moveAroundCurrFrame = 0;
    moveAroundDelay = delay;
    moveAroundAccelemetor = accelemetor;
    isMoveAround = true;

    if (accelemetor != 0)
    {
        isMoveAroundAcclemetor = true;
    }
    else
    {
        isMoveAroundAcclemetor = false;
    }

    moveAroundDirection = direction;
    setMoveAroundRadius(radius);
    setMoveAroundSpeed(speed);
}

void BossStage3Joint::moveFollow(BossStage3Joint* joint, bool isMoveFollowWithAccelemetor)
{
    jointFollow = joint;
    isMoveFollow = true;
    isMoveFollowFirstTime = true;
    this->isMoveFollowWithAccelemetor = isMoveFollowWithAccelemetor;

    if (joint->isMoveAround)
    {
        isJointFollowStoped = false;
    }
    else
    {
        isJointFollowStoped = true;
    }

    SetVX(joint->getCurrentMoveDirection().x);
    SetVY(joint->getCurrentMoveDirection().y);
    moveAroundDirection = joint->moveAroundDirection == MoveAroundDirection::Positive ? MoveAroundDirection::Negative : MoveAroundDirection::Positive;
}

void BossStage3Joint::alignDistance(BossStage3Joint* joint2)
{
    D3DXVECTOR3 vect = (this->GetPosition() - joint2->GetPosition());
    D3DXVECTOR2 vec = D3DXVECTOR2(vect.x, vect.y);
    float distance = 16;

    D3DXVec2Normalize(&vec, &vec);
    float angle = acos(vec.x) * (abs(vec.y) / vec.y);;

    float posx = cos(angle);
    float posy = sin(angle);

    float joint1X = joint2->GetPosition().x + vec.x * distance;
    float joint1Y = joint2->GetPosition().y + vec.y * distance;

    SetX(joint1X);
    SetY(joint1Y);
}

void BossStage3Joint::stopMoveAround()
{
    isMoveAround = false;
    moveAroundSpeed = 0;
    moveAroundAccelemetor = 0;
    moveAroundCurrFrame = 0;
    angleMoved = 0;
}

void BossStage3Joint::setMoveAroundSpeed(float speed)
{
    if (speed <= 0)
        moveAroundSpeed = 0;
    else
        moveAroundSpeed = speed;
}

void BossStage3Joint::setMoveAroundRadius(float radius)
{
    moveAroundRadius = radius;
}

D3DXVECTOR2 BossStage3Joint::getCurrentMoveDirection()
{
    return currMoveDirection;
}

float BossStage3Joint::getAngleSpeed()
{
    return moveAroundSpeed;
}

void BossStage3Joint::reverseMoveAroundDirection()
{
    if (moveAroundDirection == MoveAroundDirection::Positive)
    {
        moveAroundDirection = MoveAroundDirection::Negative;
    }
    else
    {
        moveAroundDirection = MoveAroundDirection::Positive;
    }

    this->angleMoved = 0;
}

void BossStage3Joint::setMoveAroundDelay(int frame)
{
    moveAroundDelay = frame;
}

void BossStage3Joint::stopMoveFollow()
{
    this->isMoveFollow = false;
    jointFollow = nullptr;
}

int BossStage3Joint::getMoveAroundCurrFrame()
{
    return moveAroundCurrFrame;
}