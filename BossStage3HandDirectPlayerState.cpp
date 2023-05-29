#include "BossStage3Hand.h"

BossStage3HandDirectPlayerState::BossStage3HandDirectPlayerState(BossStage3Hand& bossStage3hand) : BossStage3HandState()
{

}

BossStage3HandDirectPlayerState::~BossStage3HandDirectPlayerState()
{
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