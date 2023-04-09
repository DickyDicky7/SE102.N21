#include "RifleMan.h"

RifleManAppearState::RifleManAppearState()
{

}


RifleManAppearState::~RifleManAppearState()
{

}

void RifleManAppearState::Enter(RifleMan& rifleMan)
{

}

void RifleManAppearState::Exit(RifleMan& rifleMan)
{

}

void RifleManAppearState::Render(RifleMan& rifleMan)
{
	rifleMan.SetAnimation(RIFLE_MAN_ANIMATION_ID::APPEAR, rifleMan.GetPosition(), rifleMan.GetMovingDirection(), rifleMan.GetAngle());
}

RifleManState* RifleManAppearState::Update(RifleMan& rifleMan)
{
	return NULL;
}