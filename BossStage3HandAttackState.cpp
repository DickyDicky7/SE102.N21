#include "BossStage3Hand.h"

BossStage3HandAttackState::BossStage3HandAttackState(BossStage3Hand& bossStage3hand) : BossStage3HandState()
{

}

BossStage3HandAttackState::~BossStage3HandAttackState()
{
}


void BossStage3HandAttackState::Exit(BossStage3Hand& bossStage3hand)
{
}

void BossStage3HandAttackState::Enter(BossStage3Hand& bossStage3hand)
{

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
    return NULL;
}

BossStage3HandState* BossStage3HandAttackState::HandleInput(BossStage3Hand& bossStage3hand, Input& input)
{
    return NULL;
}