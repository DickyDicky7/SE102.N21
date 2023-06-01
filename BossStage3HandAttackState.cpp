#include "BossStage3Hand.h"

BossStage3HandAttackState::BossStage3HandAttackState(BossStage3Hand& bossStage3hand) : BossStage3HandState()
{

}

BossStage3HandAttackState::~BossStage3HandAttackState()
{
    delayChangeState = NULL;
    frameAttack = NULL;
    joints = NULL;
}


void BossStage3HandAttackState::Exit(BossStage3Hand& bossStage3hand)
{
}

void BossStage3HandAttackState::Enter(BossStage3Hand& bossStage3hand)
{
    float speed = 5.0f;
    float limitFrame = 100 / speed;
    frameAttack = 10;
    delayChangeState = 40;

    joints = bossStage3hand.joints;

    if (bossStage3hand.GetMovingDirection() == DIRECTION::LEFT)
    {
        moveAround(bossStage3hand.joints[1], bossStage3hand.joints[0], 0, speed,
            limitFrame, BossStage3Joint::MoveAroundDirection::Negative);
    }
    else
    {
        moveAround(bossStage3hand.joints[1], bossStage3hand.joints[0], 0, speed,
            limitFrame, BossStage3Joint::MoveAroundDirection::Positive);
    }

    bossStage3hand.joints[2]->moveFollow(bossStage3hand.joints[1]);
    bossStage3hand.joints[3]->moveFollow(bossStage3hand.joints[2]);
    bossStage3hand.joints[4]->moveFollow(bossStage3hand.joints[3]);
}

void BossStage3HandAttackState::Render(BossStage3Hand& bossStage3hand)
{
    for (size_t i = 0; i < 5; i++)
    {
        bossStage3hand.joints[i]->Render();
    }
}

BossStage3HandState* BossStage3HandAttackState::Update(BossStage3Hand& bossStage3hand)
{
    if (frameAttack > 0)
    {
        frameAttack--;

        if (frameAttack == 0)
        {
            // Nhet dan vao
            FLOAT xBill = bossStage3hand.GetTarget()->GetPosition().x;
            FLOAT yBill = bossStage3hand.GetTarget()->GetPosition().y;
            D3DXVECTOR3 j = bossStage3hand.joints[4]->GetPosition();
            float angle = getAngle(D3DXVECTOR2(xBill, yBill), D3DXVECTOR2(j.x, j.y));
            //bossStage3hand.bulletsVector.push_back(new Boss2FinalBullet(joints[4]->getPosition().x, joints[4]->getPosition().y, 1.5f, angle));
        }
    }

    if (bossStage3hand.joints[1]->isMoveAround == false)
    {
        bossStage3hand.joints[2]->moveAroundSpeed *= 0.99;
    }

    if (bossStage3hand.joints[2]->isMoveAround == false)
    {
        bossStage3hand.joints[3]->moveAroundSpeed *= 1.1;
    }

    if (bossStage3hand.joints[4]->isMoveAround == false && bossStage3hand.joints[1]->isMoveAround == false)
    {
        if (delayChangeState >= 0)
        {
            delayChangeState--;
        }
        else
        {
            //return new BossStage3HandDirectPlayerState(bossStage3hand);
        }
    }

    for (size_t i = 0; i < 5; i++)
    {
        bossStage3hand.joints[i]->Update();
    }

    return NULL;
}

BossStage3HandState* BossStage3HandAttackState::HandleInput(BossStage3Hand& bossStage3hand, Input& input)
{
    return NULL;
}

void BossStage3HandAttackState::moveAround(BossStage3Joint* joint, BossStage3Joint* joint0, float deltaRadius,
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

float BossStage3HandAttackState::getAngle(D3DXVECTOR2 pos1, D3DXVECTOR2 pos2)
{
    D3DXVECTOR2 vec(pos1 - pos2);
    D3DXVec2Normalize(&vec, &vec);

    float angle = acos(vec.x) * (abs(vec.y) / vec.y);

    return angle;
}