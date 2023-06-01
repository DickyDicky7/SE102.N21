#include "BossStage3Hand.h"

BossStage3HandWaveState::BossStage3HandWaveState(BossStage3Hand& bossStage3hand) : BossStage3HandState()
{

}

BossStage3HandWaveState::~BossStage3HandWaveState()
{
    limitFrame = NULL;
    speed = NULL;
    currFrame = NULL; 
    changeStateFrame = NULL;
    moveAroundDelay = NULL;
    listJoint1Angle = std::vector<D3DXVECTOR2>();
}


void BossStage3HandWaveState::Exit(BossStage3Hand& bossStage3hand)
{
}

void BossStage3HandWaveState::Enter(BossStage3Hand& bossStage3hand)
{
    moveAroundDelay = 0;

    float x = bossStage3hand.joints[0]->GetX() + bossStage3hand.joints[0]->GetW() / 2;
    float y = bossStage3hand.joints[0]->GetY() + bossStage3hand.joints[0]->GetH() / 2;

    speed = 5.0f;
    currFrame = 0;

    if (bossStage3hand.GetMovingDirection() == DIRECTION::LEFT)
    {
        moveAround(bossStage3hand.joints[1], bossStage3hand.joints[0], 0, speed,
            -1, BossStage3Joint::MoveAroundDirection::Negative);
    }
    else
    {
        moveAround(bossStage3hand.joints[1], bossStage3hand.joints[0], 0, speed,
            -1, BossStage3Joint::MoveAroundDirection::Positive);
    }

    bossStage3hand.joints[2]->moveFollow(bossStage3hand.joints[1]);
    bossStage3hand.joints[3]->moveFollow(bossStage3hand.joints[2]);
    bossStage3hand.joints[4]->moveFollow(bossStage3hand.joints[3]);
}

void BossStage3HandWaveState::Render(BossStage3Hand& bossStage3hand)
{
    for (size_t i = 0; i < 5; i++)
    {
        bossStage3hand.joints[i]->Render();
    }
}

BossStage3HandState* BossStage3HandWaveState::Update(BossStage3Hand& bossStage3hand)
{
    D3DXVECTOR3 a = bossStage3hand.joints[1]->GetPosition();
    D3DXVECTOR3 b = bossStage3hand.joints[0]->GetPosition();

    float angle = getAngle(D3DXVECTOR2(a.x, a.y), D3DXVECTOR2(b.x,b.y));

    switch (bossStage3hand.joints[1]->moveAroundDirection)
    {
    case BossStage3Joint::MoveAroundDirection::Positive:
        //doi huong chuyen dong

        if (bossStage3hand.GetMovingDirection() == DIRECTION::LEFT)
        {
            //nguoc chuyen huong chuyen dong cua tay trai
            //positive la tay trai quay len tren
            if (angle < 0 && angle > D3DXToRadian(-60))
            {
                currFrame++;
                bossStage3hand.joints[1]->setMoveAroundSpeed(speed);
                bossStage3hand.joints[1]->reverseMoveAroundDirection();
                bossStage3hand.joints[1]->setMoveAroundDelay(moveAroundDelay);
            }
        }
        else
        {
            //tay phai
            //positive la tay phai quay xuong duoi
            if (angle > 0 && angle > D3DXToRadian(100))
            {
                currFrame++;
                bossStage3hand.joints[1]->setMoveAroundSpeed(speed);
                bossStage3hand.joints[1]->reverseMoveAroundDirection();
                bossStage3hand.joints[1]->setMoveAroundDelay(moveAroundDelay);
            }
        }

        break;

    case BossStage3Joint::MoveAroundDirection::Negative:
        //doi huong chuyen dong
        //nguoc lai voi positive
        if (bossStage3hand.GetMovingDirection() == DIRECTION::LEFT)
        {
            if (angle > 0 && angle < D3DXToRadian(80))
            {
                currFrame++;
                bossStage3hand.joints[1]->setMoveAroundSpeed(speed);
                bossStage3hand.joints[1]->reverseMoveAroundDirection();
                bossStage3hand.joints[1]->setMoveAroundDelay(moveAroundDelay);
            }
        }
        else
        {
            if (angle < 0 && angle < D3DXToRadian(-120))
            {
                currFrame++;
                bossStage3hand.joints[1]->setMoveAroundSpeed(speed);
                bossStage3hand.joints[1]->reverseMoveAroundDirection();
                bossStage3hand.joints[1]->setMoveAroundDelay(moveAroundDelay);
            }
        }
        break;
    }

    if (currFrame >= 6)
    {
        return new BossStage3HandSpinningState(bossStage3hand);
    }

    for (size_t i = 0; i < 5; i++)
    {
        bossStage3hand.joints[i]->Update();
    }
    return NULL;
}

BossStage3HandState* BossStage3HandWaveState::HandleInput(BossStage3Hand& bossStage3hand, Input& input)
{
    return NULL;
}


void BossStage3HandWaveState::moveAround(BossStage3Joint* joint, BossStage3Joint* joint0, float deltaRadius,
    float speed, float frame, BossStage3Joint::MoveAroundDirection dir, float delay)
{
    float x = joint0->GetX() + joint0->GetW() / 2;
    float y = joint0->GetY() + joint0->GetH() / 2;

    float x2 = joint->GetX() + joint->GetW() / 2;
    float y2 = joint->GetY() + joint->GetH() / 2;

    D3DXVECTOR2 dist(x - x2, y - y2);

    //joint ngoai cung se dung lai truoc joint trong cung va hand dung lai dau tien
    //thu hep khoang cach cua joint va hand lai vi xoay khoang cach bi gian ra
    //de tao hieu ung wave thi van toc cua cac joint phai khac nhau va joint trong cung di chuyen nhanh nhat
    joint->moveAround(x, y, D3DXVec2Length(&dist) - deltaRadius, frame, speed, dir, delay);
}

float BossStage3HandWaveState::getAngle(D3DXVECTOR2 pos1, D3DXVECTOR2 pos2)
{
    D3DXVECTOR2 vec(pos1 - pos2);
    D3DXVec2Normalize(&vec, &vec);

    float angle = acos(vec.x) * (abs(vec.y) / vec.y);

    return angle;
}