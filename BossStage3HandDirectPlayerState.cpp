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

    float angle = acos(vec.x) * (abs(vec.y) / vec.y);

    return angle;
}

D3DXVECTOR3 BossStage3HandDirectPlayerState::getNearestPlayer(BossStage3Hand& bossStage3hand)
{
    FLOAT xBill = bossStage3hand.GetTarget()->GetPosition().x;
    FLOAT yBill = bossStage3hand.GetTarget()->GetPosition().y;
    return  D3DXVECTOR3(xBill, yBill, 0);
}