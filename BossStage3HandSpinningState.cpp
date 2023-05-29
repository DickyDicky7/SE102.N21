#include "BossStage3Hand.h"

BossStage3HandSpinningState::BossStage3HandSpinningState(BossStage3Hand& bossStage3hand) : BossStage3HandState()
{
}

BossStage3HandSpinningState::~BossStage3HandSpinningState()
{
}


void BossStage3HandSpinningState::Exit(BossStage3Hand& bossStage3hand)
{
}

void BossStage3HandSpinningState::Enter(BossStage3Hand& bossStage3hand)
{
    frameAttack = FRAME_ATTACK;

    float x = bossStage3hand.joints[0]->GetX() + bossStage3hand.joints[0]->GetW() / 2;
    float y = bossStage3hand.joints[0]->GetY() + bossStage3hand.joints[0]->GetH() / 2;

    speedAngle = 5.7f;
    totalFrame = 400 / speedAngle;
    delayChangeState = 6;

    if (bossStage3hand.GetMovingDirection() == DIRECTION::LEFT)
    {
        moveAround(bossStage3hand.joints[1], bossStage3hand.joints[0], 0, speedAngle,
            totalFrame, BossStage3Joint::MoveAroundDirection::Positive);
    }
    else
    {
        moveAround(bossStage3hand.joints[1], bossStage3hand.joints[0], 0, speedAngle,
            totalFrame, BossStage3Joint::MoveAroundDirection::Negative);
    }

    joints = bossStage3hand.joints;
}

void BossStage3HandSpinningState::moveAround(BossStage3Joint* joint, BossStage3Joint* joint0, float deltaRadius,
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


void BossStage3HandSpinningState::Render(BossStage3Hand& bossStage3hand)
{
    for (size_t i = 0; i < 5; i++)
    {
        bossStage3hand.joints[i]->Render();
    }
}

BossStage3HandState* BossStage3HandSpinningState::Update(BossStage3Hand& bossStage3hand)
{
    if (frameAttack >= 0)
    {
        frameAttack--;
    }
    else
    {
        float angle = 0;
        
        // lay goc thang bill de ban
        FLOAT xBill = bossStage3hand.GetTarget()->GetPosition().x;
        FLOAT yBill = bossStage3hand.GetTarget()->GetPosition().y;
        D3DXVECTOR3 j = bossStage3hand.joints[4]->GetPosition();
        angle = getAngle(D3DXVECTOR2(xBill, yBill), D3DXVECTOR2(j.x, j.y));

        // Nhet dan vao
        //bossStage3hand.bulletsVector.push_back(new Boss2FinalBullet(joints[4]->getPosition().x, joints[4]->getPosition().y, 1.0f, angle));
        frameAttack = FRAME_ATTACK;
    }

    if (!bossStage3hand.joints[1]->isMoveAround)
    {
        bossStage3hand.joints[4]->setMoveAroundSpeed(bossStage3hand.joints[4]->moveAroundSpeed * 0.99f);
    }

    if (!bossStage3hand.joints[4]->isMoveAround)
    {
        if (delayChangeState >= 0)
        {
            delayChangeState--;
        }
        else
        {
            return new BossStage3HandAttackState(bossStage3hand);
        }
    }

    for (size_t i = 0; i < 5; i++)
    {
        bossStage3hand.joints[i]->Update();
    }
    return NULL;
}

BossStage3HandState* BossStage3HandSpinningState::HandleInput(BossStage3Hand& bossStage3hand, Input& input)
{
    return NULL;
}

float BossStage3HandSpinningState::getAngle(D3DXVECTOR2 pos1, D3DXVECTOR2 pos2)
{
    D3DXVECTOR2 vec(pos1 - pos2);
    D3DXVec2Normalize(&vec, &vec);

    float angle = acos(vec.x) * (abs(vec.y) / vec.y);

    return angle;
}