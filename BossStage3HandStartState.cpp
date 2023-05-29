#include "BossStage3Hand.h"

BossStage3HandStartState::BossStage3HandStartState(BossStage3Hand& bossStage3hand) : BossStage3HandState()
{
    
}

BossStage3HandStartState::~BossStage3HandStartState()
{
}


void BossStage3HandStartState::Exit(BossStage3Hand& bossStage3hand)
{
}

void BossStage3HandStartState::Enter(BossStage3Hand& bossStage3hand)
{
    delayFrame = 80;

    if (bossStage3hand.GetMovingDirection() == DIRECTION::LEFT)
    {
        direction = D3DXVECTOR2(cos(D3DXToRadian(110)), sin(D3DXToRadian(110))); //huong di 
    }
    else
    {
        direction = D3DXVECTOR2(cos(D3DXToRadian(70)), sin(D3DXToRadian(70))); //huong di 
    }

    distance = 4 * 16; //quang duong di chuyen
    speedFrame = distance / D3DXVec2Length(&direction); // quang duong / frame
    float t = 16 / D3DXVec2Length(&direction); // thoi gian di het 16pixel - de tinh thoi gian delay cua joint ke tiep

    for (size_t i = 4; i > 0; i--)
    {
        bossStage3hand.joints[i]->moveBy(direction.x, direction.y, speedFrame - (3 - i + 1) * t, (3 - i + 1) * t);
    }
}

void BossStage3HandStartState::Render(BossStage3Hand& bossStage3hand)
{
	for (size_t i = 0; i < 5; i++)
	{
		bossStage3hand.joints[i]->Render();
	}
}

BossStage3HandState* BossStage3HandStartState::Update(BossStage3Hand& bossStage3hand)
{
    if (delayFrame > 0)
    {
        delayFrame--;
        return NULL;
    }

    for (size_t i = 0; i < 5; i++)
    {
        bossStage3hand.joints[i]->Update();
    }

    if ( bossStage3hand.joints[4]->isMoveBy == false)
    {
        return new BossStage3HandWaveState(bossStage3hand);
    }
	return NULL;
}

BossStage3HandState* BossStage3HandStartState::HandleInput(BossStage3Hand& bossStage3hand, Input& input)
{
	return NULL;
}