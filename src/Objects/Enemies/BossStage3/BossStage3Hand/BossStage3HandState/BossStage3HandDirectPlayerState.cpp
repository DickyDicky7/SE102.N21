#include "BossStage3Hand.h"

BossStage3HandDirectPlayerState::BossStage3HandDirectPlayerState(BossStage3Hand& bossStage3hand) : BossStage3HandState()
{

}

BossStage3HandDirectPlayerState::~BossStage3HandDirectPlayerState()
{
    joints = NULL;
    speed = NULL;
    isFirstTime = NULL;
    frameDelayChangeState = NULL;
    timeAttack = NULL;
}


void BossStage3HandDirectPlayerState::Exit(BossStage3Hand& bossStage3hand)
{
}

void BossStage3HandDirectPlayerState::Enter(BossStage3Hand& bossStage3hand)
{
    isFirstTime = true;

    timeAttack = 60;

    joints = bossStage3hand.joints;
    speed = 5.0f;
    frameDelayChangeState = 280;

    for (size_t i = 2; i < 5; i++)
    {
        joints[i]->moveFollow(joints[i - 1], false);
    }

    for (size_t i = 1; i < 5; i++)
    {
        joints[i]->stopMoveFollow();
        moveAroundDirect(bossStage3hand, joints[0], joints[i], speed, 15 * i);
    }
}

void BossStage3HandDirectPlayerState::Render(BossStage3Hand& bossStage3hand)
{
    for (size_t i = 0; i < 5; i++)
    {
        bossStage3hand.joints[i]->Render();
    }
}

BossStage3HandState* BossStage3HandDirectPlayerState::Update(BossStage3Hand& bossStage3hand)
{
    if (timeAttack > 0)
    {
        timeAttack--;

        if (timeAttack == 0)
        {
            D3DXVECTOR3 a = getNearestPlayer(bossStage3hand);
            D3DXVECTOR3 b = bossStage3hand.joints[4]->GetPosition();

            float angle = getAngle(D3DXVECTOR2(a.x, a.y), D3DXVECTOR2(b.x, b.y));

            bossStage3hand.Fire(bossStage3hand.joints[4]->GetX(), bossStage3hand.joints[4]->GetY(), angle, -0.5f);
        }
    }

    frameDelayChangeState--;

    if (frameDelayChangeState <= 0)
    {
        return new BossStage3HandWaveState(bossStage3hand);
        return NULL;
    }

    if (!joints[1]->isMoveAround)
    {
        isFirstTime = false;
    }

    for (size_t i = 1; i < 5; i++)
    {
        if (!joints[i]->isMoveAround)
        {
            if (abs(getAngleBetweenPlayerAndjoint(joints[i], bossStage3hand)) >= 5.0f)
            {
                moveAroundDirect(bossStage3hand, joints[0], joints[i], speed, 15 * i);
            }
        }
    }

    for (size_t i = 2; i < 5; i++)
    {
        joints[i]->alignDistance(joints[i - 1]);
    }

    for (size_t i = 0; i < 5; i++)
    {
        bossStage3hand.joints[i]->Update();
    }
    return NULL;
}

BossStage3HandState* BossStage3HandDirectPlayerState::HandleInput(BossStage3Hand& bossStage3hand, Input& input)
{
    return NULL;
}


float BossStage3HandDirectPlayerState::getAngle2Vector(D3DXVECTOR3 vec31, D3DXVECTOR3 vec32)
{
    D3DXVECTOR2 vec1 = D3DXVECTOR2(vec31.x, vec31.y);
    D3DXVECTOR2 vec2 = D3DXVECTOR2(vec32.x, vec32.y);

    D3DXVec2Normalize(&vec1, &vec1);
    D3DXVec2Normalize(&vec2, &vec2);

    float angle1 = acos(vec1.x) * (abs(vec1.y) / vec1.y);
    float angle2 = acos(vec2.x) * (abs(vec2.y) / vec2.y);;

    float result = angle2 - angle1;

    return result;
}

float BossStage3HandDirectPlayerState::getAngleBetweenPlayerAndjoint(BossStage3Joint* joint, BossStage3Hand& bossStage3hand)
{
    D3DXVECTOR3 playerPos;
    playerPos = getNearestPlayer(bossStage3hand);

    float angle = D3DXToDegree(getAngle2Vector(joint->GetPosition() - joints[0]->GetPosition(), playerPos - joints[0]->GetPosition()));

    return angle;
}

void BossStage3HandDirectPlayerState::moveAroundDirect(BossStage3Hand& bossStage3hand, BossStage3Joint* joint0, BossStage3Joint* joint, float speed, float radius)
{
    D3DXVECTOR3 playerPos;
    playerPos = getNearestPlayer(bossStage3hand);

    float angle = D3DXToDegree(getAngle2Vector(joint->GetPosition() - joints[0]->GetPosition(), playerPos - joints[0]->GetPosition()));

    int frameMove = abs(angle / speed);

    if (isFirstTime)
    {
        //lan dau tien tay trai se di ve ben trai, tay phai di huong ben phai
        //lan sau thi di chuyen theo do lech 
        //do lech goc
        if (bossStage3hand.GetMovingDirection() == DIRECTION::LEFT)
        {
            if (angle > 0)
                frameMove = abs((360 - angle) / speed);

            joint->moveAround(joint0->GetPosition().x, joint0->GetPosition().y, radius, frameMove, speed, BossStage3Joint::MoveAroundDirection::Negative);
        }
        else
        {
            joint->moveAround(joint0->GetPosition().x, joint0->GetPosition().y, radius, frameMove, speed, BossStage3Joint::MoveAroundDirection::Positive);
        }
    }
    else
    {
        if (angle < 0)
        {
            joint->moveAround(joint0->GetPosition().x, joint0->GetPosition().y, radius, frameMove, speed, BossStage3Joint::MoveAroundDirection::Negative);
        }
        else
        {
            joint->moveAround(joint0->GetPosition().x, joint0->GetPosition().y, radius, frameMove, speed, BossStage3Joint::MoveAroundDirection::Positive);
        }
    }
}


float BossStage3HandDirectPlayerState::getAngle(D3DXVECTOR2 pos1, D3DXVECTOR2 pos2)
{
    D3DXVECTOR2 vec(pos1 - pos2);
    D3DXVec2Normalize(&vec, &vec);

   // float angle = acos(vec.x) * (abs(vec.y) / vec.y);
    float angle = - vec.x / vec.y;
    return angle;
}

D3DXVECTOR3 BossStage3HandDirectPlayerState::getNearestPlayer(BossStage3Hand& bossStage3hand)
{
    FLOAT xBill = bossStage3hand.GetTarget()->GetPosition().x;
    FLOAT yBill = bossStage3hand.GetTarget()->GetPosition().y;
    return  D3DXVECTOR3(xBill, yBill, 0);
}