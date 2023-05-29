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
    return NULL;
}

BossStage3HandState* BossStage3HandSpinningState::HandleInput(BossStage3Hand& bossStage3hand, Input& input)
{
    return NULL;
}