#include "AirCraft.h"

AirCraftNormalState::AirCraftNormalState() : AirCraftState()
{
}

AirCraftNormalState::~AirCraftNormalState()
{
}


void AirCraftNormalState::Exit(AirCraft& aircraft)
{
}

void AirCraftNormalState::Enter(AirCraft& aircraft)
{
}

void AirCraftNormalState::Render(AirCraft& aircraft)
{
	aircraft.
}

AirCraftState* AirCraftNormalState::Update(AirCraft& aircraft)
{
	
	

	return NULL;
}

AirCraftState* AirCraftNormalState::HandleInput(AirCraft& aircraft, Input& input)
{
	//invul, b, f, l, m, r, s
	if (input.IsKey(DIK_1)) {
		if (aircraft.getAmmoType() == ITEM_TYPE::I)
		{
			return new AirCraftInvulState();
		}
		else if (aircraft.getAmmoType() == ITEM_TYPE::B)
		{
			return new AirCraftBAmmoState();
		}
		else if (aircraft.getAmmoType() == ITEM_TYPE::F)
		{
			return new AirCraftFAmmoState();
		}
		else if (aircraft.getAmmoType() == ITEM_TYPE::L)
		{
			return new AirCraftLAmmoState();
		}
		else if (aircraft.getAmmoType() == ITEM_TYPE::M)
		{
			return new AirCraftMAmmoState();
		}
		else if (aircraft.getAmmoType() == ITEM_TYPE::R)
		{
			return new AirCraftRAmmoState();
		}
		else if (aircraft.getAmmoType() == ITEM_TYPE::S)
		{
			return new AirCraftSAmmoState();
		}
	}
	
	
	return NULL;
}