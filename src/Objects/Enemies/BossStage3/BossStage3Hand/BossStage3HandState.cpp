#include "BossStage3Hand.h"

BossStage3HandState::BossStage3HandState()
{
}

BossStage3HandState::~BossStage3HandState()
{
}

void BossStage3HandState::Render(BossStage3Hand& bossStage3Hand)
{
	for (size_t i = 0; i < Constants::Enemies::BossStage3::Hand::TOTAL_JOINTS_COUNT; i++)
	{
		bossStage3Hand.GetJoint(i)->Render();
	}
}

float BossStage3HandState::GetAngle(D3DXVECTOR2 pos1, D3DXVECTOR2 pos2)
{
	D3DXVECTOR2 vec(pos1 - pos2);
	D3DXVec2Normalize(&vec, &vec);

	if (vec.y == 0.0f)
	{
		return 0.0f;
	}
	return -vec.x / vec.y;
}
