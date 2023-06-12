#pragma once
#include "Bill.h"
#include "Enemy.h"
#include "State.h"
#include "Common.h"
#include "Entity.h"
#include "HasSprites.h"
#include "HasTextures.h"
#include "HasAnimations.h"
#include "ItemCommon.h"W

class AirCraft : public Entity, public Enemy<Bill>
	, public HasTextures<AirCraft>, public HasSprites<AirCraft>, public HasAnimations<AirCraft>
{
public:

	AirCraft(ITEM_TYPE, AIRCRAFT_DIRECTION);
	virtual ~AirCraft();
	void Update() override;
	void Render() override;
	void HandleInput(Input&) override;

	void LoadSprites() override;
	void LoadTextures() override;
	void LoadAnimations() override;

	ITEM_TYPE getAmmoType();
	void setAmmoType(ITEM_TYPE);

	AIRCRAFT_DIRECTION getAircarftDirection() 
	{
		return _aircarftDirection;
	}
	void setAircarftDirection(AIRCRAFT_DIRECTION direction) {
		_aircarftDirection = direction;
	}
protected:

	ITEM_TYPE _ammoType;
	AIRCRAFT_DIRECTION _aircarftDirection; // huong ngang hoac doc

	FLOAT time;
	FLOAT y0;
	FLOAT x0;
	FLOAT dt;
	FLOAT T;
	FLOAT A;
	FLOAT φ;

	// x0, y0: vị trí lúc đầu
	// time: thời gian
	// dt: delta time ( t = t + dt )
	// T: khoảng thời gian để quay hết 1 vòng (tính bằng giây)
	// A: Bán kính
	// φ: pha ban đầu của dao động (-π<φ<π)
};