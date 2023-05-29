#include "BossStage3Hand.h"

BossStage3HandWaveState::BossStage3HandWaveState(BossStage3Hand& bossStage3hand) : BossStage3HandState()
{

}

BossStage3HandWaveState::~BossStage3HandWaveState()
{
}


void BossStage3HandWaveState::Exit(BossStage3Hand& bossStage3hand)
{
}

void BossStage3HandWaveState::Enter(BossStage3Hand& bossStage3hand)
{
    
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
    return NULL;
}

BossStage3HandState* BossStage3HandWaveState::HandleInput(BossStage3Hand& bossStage3hand, Input& input)
{
    return NULL;
}